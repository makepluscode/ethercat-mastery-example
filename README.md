# XMC4800 EtherCAT Master Lab

EtherCAT scan부터 PDO 통신까지 단계적으로 확장하는 최소 프로젝트입니다.

## 구조

```text
host/                 Linux C++17 raw-socket EtherCAT master 도구
firmware/             XMC4800 Relax EtherCAT 펌웨어, RTT 로그
scripts/              Ubuntu 설정, 빌드, 플래시, RTT, lint 헬퍼
third_party/          다운로드한 ModusToolbox 라이브러리, git 제외
tools/                다운로드한 Arm GNU toolchain, git 제외
```

## Ubuntu 설정

```bash
sudo apt-get install -y git curl ca-certificates
./scripts/setup_ubuntu.sh
```

`setup_ubuntu.sh`는 필요한 Ubuntu 도구를 설치하고, Arm GNU Toolchain 14.2를 `tools/`에 다운로드하며,
필요한 ModusToolbox 라이브러리를 `third_party/mtb_shared`에 가져옵니다.

전체 ModusToolbox IDE 설치는 선택 사항입니다. 이 저장소의 펌웨어 빌드는 체크인된 Modus 프로젝트 파일과
Linux용 Ninja 변환 스크립트를 사용하므로, 일반적인 CLI 빌드/플래시에는 GUI 설치가 필요하지 않습니다.

## 빌드

```bash
./scripts/build_host.sh
./scripts/build_firmware.sh
```

펌웨어 산출물:

```text
firmware/build/APP_KIT_XMC48_RELAX_ECAT_V1/Debug/mtb-example-xmc48-ethernet-ethcat-loopback.elf
firmware/build/APP_KIT_XMC48_RELAX_ECAT_V1/Debug/mtb-example-xmc48-ethernet-ethcat-loopback.hex
```

## 플래시와 로그

XMC4800 Relax EtherCAT kit의 debug USB를 연결합니다.

```bash
./scripts/flash_firmware.sh
./scripts/rtt_log.sh
./scripts/rtt_log_jlink.sh
```

`rtt_log.sh`는 OpenOCD 경로, `rtt_log_jlink.sh`는 J-Link RTT Logger 경로입니다.

예상 RTT 로그에는 다음과 같은 줄이 포함됩니다.

```text
ETH/ECAT logs enabled: ECAT IN for WKC scan, X400 for raw RX probe frames.
ECAT: p0_link=... p1_link=... al=... al_event=... status=...
```

## 스캔

PC NIC를 `ECAT IN`에 연결합니다.

```bash
sudo setcap cap_net_raw+ep build/host/ecat_scan_xmc4800
./build/host/ecat_scan_xmc4800 eno1 8
```

현재 scanner(`host/src/main.cpp`)는 raw EtherCAT `BRD`/`APRD` datagram을 보내고 WKC와 기본 ESC register 값을 출력합니다.

## Lint

```bash
./scripts/lint.sh
```

이 명령은 `clang-format --dry-run`을 실행하고, `build/host`가 있으면 host `clang-tidy`도 실행합니다.

## 현재 범위

- 구현됨: EtherCAT datagram command layer, raw EtherCAT scan, board RTT ECAT 상태 로그.
- 다음 단계: AL state control, mailbox/SII read, PDO mapping, cyclic process data.

## 학습 로드맵

Host master는 작고 검증 가능한 단계로 확장합니다. 각 단계가 끝난 뒤에도 이전 scan 경로는 계속 동작해야 합니다.

### 1. EtherCAT Datagram Command

목표: 상위 protocol logic을 추가하기 전에 raw EtherCAT frame/datagram 계층을 명확히 분리합니다.

- 상태: 구현됨.
- Frame 생성, datagram header parsing, data access, WKC parsing을 작은 helper로 분리합니다.
- Command 지원은 `BRD`, `BWR`, `APRD`, `APWR`, `FPRD`, `FPWR`, 이후 `LRD`, `LWR`, `LRW` 순서로 확장합니다.
- 처음에는 Ethernet frame 하나에 datagram 하나만 담고, 나중에 multiple datagram으로 확장할 수 있는 API 형태를 유지합니다.
- 각 command는 알려진 ESC register로 검증하고 command, address, data, WKC를 출력합니다.

### 2. Scan

목표: slave를 안정적으로 찾고 broadcast, auto-increment, fixed addressing의 차이를 학습합니다.

- `BRD` 기반 WKC로 slave 수를 확인하는 방식을 유지합니다.
- 각 slave의 ESC type, revision, AL status, DL status, station address, alias를 `APRD`로 읽습니다.
- Scan 순서대로 configured station address를 할당합니다.
- Station address 할당 후 `FPRD`로 slave register를 다시 읽습니다.
- 검출된 slave 수, 할당된 address, `FPRD` read 결과가 서로 일관적인지 확인합니다.

### 3. AL State 제어

목표: 실패 원인을 확인할 수 있게 slave를 EtherCAT Application Layer state 사이에서 전이시킵니다.

- AL Control, AL Status, AL Status Code register 접근을 추가합니다.
- `INIT`, `PRE-OP`, `SAFE-OP`, `OP` state request를 구현합니다.
- 요청한 state에 도달하거나 timeout/error status가 나올 때까지 polling합니다.
- 각 전이마다 target state, actual state, status code, WKC를 출력해 검증합니다.

### 4. SII EEPROM Read

목표: mailbox 또는 PDO 기능을 사용하기 전에 SII에서 slave identity와 configuration data를 읽습니다.

- Busy, timeout, error bit 처리를 포함해 ESC EEPROM/SII register 접근을 구현합니다.
- Vendor ID, product code, revision, serial number 같은 identity word를 읽습니다.
- Mailbox, SyncManager, PDO 관련 정보를 찾을 수 있을 만큼 SII category를 parsing합니다.
- 사람이 읽기 쉬운 identity 요약과 raw SII word dump를 함께 출력해 검증합니다.

### 5. CoE SDO

목표: mailbox communication으로 Object Dictionary entry를 읽고 씁니다.

- SII data를 기반으로 mailbox SyncManager를 설정하거나 현재 설정을 확인합니다.
- CoE mailbox request/response framing을 구현합니다.
- 먼저 expedited SDO upload를 구현합니다.
- Read 동작이 안정화된 뒤 expedited SDO download를 추가합니다.
- Identity object `0x1018` read로 검증하고, write는 안전한 writable object에만 수행합니다.

### 6. PDO 준비

목표: slave의 기본 PDO mapping에서 process data layout을 계산합니다.

- SII 또는 SDO로 RxPDO/TxPDO mapping을 읽습니다.
- SyncManager 2/3과 FMMU 요구사항을 모델링합니다.
- Logical process data address를 할당하고 slave별 input/output offset을 계산합니다.
- 첫 버전에서는 PDO remapping을 하지 않고 각 slave의 기본 mapping을 사용합니다.
- Cyclic exchange에 들어가기 전에 예상 process data layout을 출력해 검증합니다.

### 7. Cyclic PDO 교환

목표: 기대 WKC를 만족하면서 process data를 주기적으로 교환합니다.

- `LRD`, `LWR`, 또는 `LRW`로 logical process data access를 구현합니다.
- `SAFE-OP`에서 output을 활성화하기 전에 input PDO read를 검증합니다.
- SyncManager, FMMU, process data layout 설정이 끝난 뒤에만 `OP`로 전이합니다.
- Best-effort Linux cyclic loop에서 cycle time, WKC, missed-cycle count, input/output hex dump를 출력합니다.
- 기대 WKC가 안정적으로 나오고 process data 변화가 보이는지 확인합니다.
