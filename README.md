# DataPersistence-Andrew

> JSON 파일 기반 데이터 영속성 처리 PoC — C++17 / Visual Studio 2022

---

## 목차

1. [프로젝트 개요](#1-프로젝트-개요)
2. [요구사항](#2-요구사항)
3. [기술 스택](#3-기술-스택)
4. [개발 환경 준비](#4-개발-환경-준비)
5. [소스 구조](#5-소스-구조)
6. [아키텍처](#6-아키텍처)
7. [데이터 모델](#7-데이터-모델)
8. [빌드 및 실행](#8-빌드-및-실행)
9. [기능 사용법](#9-기능-사용법)
10. [JSON 저장 형식](#10-json-저장-형식)
11. [문서](#11-문서)

---

## 1. 프로젝트 개요

콘솔 애플리케이션에서 **JSON 파일을 통해 데이터를 저장·불러오는 구조**를 구현한 PoC(Proof of Concept)입니다.

외부 라이브러리 없이 자체 JSON 파서와 직렬화 로직을 포함하여, 아이템 데이터에 대한 완전한 **CRUD(Create / Read / Update / Delete)** 기능을 제공합니다.

---

## 2. 요구사항

### 기능 요구사항

| ID | 요구사항 | 구현 위치 |
|----|----------|-----------|
| FR-01 | 아이템 데이터를 JSON 파일에 저장할 수 있어야 한다 | `JsonStorage::save` |
| FR-02 | JSON 파일에서 아이템 데이터를 불러올 수 있어야 한다 | `JsonStorage::load` |
| FR-03 | 새 아이템을 추가(Create)할 수 있어야 한다 | `ItemRepository::create` |
| FR-04 | 전체 아이템 목록을 조회(Read)할 수 있어야 한다 | `ItemRepository::findAll` |
| FR-05 | ID로 특정 아이템을 조회(Read)할 수 있어야 한다 | `ItemRepository::findById` |
| FR-06 | 기존 아이템을 수정(Update)할 수 있어야 한다 | `ItemRepository::update` |
| FR-07 | 아이템을 삭제(Delete)할 수 있어야 한다 | `ItemRepository::remove` |
| FR-08 | 아이템 ID는 자동으로 증가해야 한다 | `ItemRepository::nextId` |
| FR-09 | 생성 일시를 자동으로 기록해야 한다 | `ItemRepository::currentTimestamp` |
| FR-10 | 변경 사항은 즉시 파일에 반영되어야 한다 | `ItemRepository::persist` |

### 비기능 요구사항

| ID | 요구사항 |
|----|----------|
| NFR-01 | 외부 라이브러리 없이 동작해야 한다 (자체 JSON 파서 구현) |
| NFR-02 | C++17 표준을 준수해야 한다 |
| NFR-03 | Visual Studio 2022 환경에서 빌드·실행이 가능해야 한다 |
| NFR-04 | 콘솔 기반 인터페이스로 동작해야 한다 |
| NFR-05 | JSON 파일은 사람이 읽을 수 있는 형식(pretty-print)으로 저장해야 한다 |
| NFR-06 | 데이터 파일이 없는 초기 상태에서도 정상 동작해야 한다 |

---

## 3. 기술 스택

| 항목 | 내용 |
|------|------|
| 언어 | C++17 |
| IDE | Visual Studio 2022 |
| 컴파일러 | MSVC v143 (Visual C++) |
| 플랫폼 | Windows x64 |
| JSON 처리 | 자체 구현 (외부 의존성 없음) |
| 파일 시스템 | `std::filesystem` (C++17) |
| 문자 인코딩 | UTF-8 (`/utf-8` 컴파일러 플래그) |

---

## 4. 개발 환경 준비

### 필수 설치 항목

- **Visual Studio 2022** (Community 이상)
  - 워크로드: `C++를 사용한 데스크톱 개발`
  - Windows 10 SDK 이상

### 설치 확인

Visual Studio Installer를 열어 아래 구성 요소가 포함되어 있는지 확인합니다.

```
워크로드 > C++를 사용한 데스크톱 개발
  ✓ MSVC v143 - VS 2022 C++ x64/x86 빌드 도구
  ✓ Windows 10 또는 11 SDK
  ✓ C++ CMake tools (선택)
```

---

## 5. 소스 구조

```
DataPersistence-Andrew/
│
├── README.md                            ← 프로젝트 설명 (이 파일)
├── CLAUDE.md                            ← AI 협업 컨텍스트 문서
├── DataPersistence.sln                  ← Visual Studio 솔루션
│
├── DataPersistence/                     ← VS 프로젝트 루트
│   ├── DataPersistence.vcxproj          ← 빌드 설정 (C++17, x64, /utf-8)
│   ├── DataPersistence.vcxproj.filters  ← 솔루션 탐색기 파일 그룹 설정
│   ├── DataPersistence.vcxproj.user     ← 디버그 작업 디렉토리 설정
│   │
│   └── src/                             ← 소스 파일
│       ├── main.cpp                     ← 진입점, 콘솔 메뉴 UI
│       ├── Item.h                       ← 데이터 모델 구조체
│       ├── JsonStorage.h                ← JSON I/O 인터페이스
│       ├── JsonStorage.cpp              ← JSON 파서 및 직렬화 구현
│       ├── ItemRepository.h             ← CRUD 인터페이스
│       └── ItemRepository.cpp           ← CRUD 비즈니스 로직 구현
│
├── data/
│   └── items.json                       ← 런타임 데이터 파일 (자동 생성)
│
├── docs/
│   ├── commit-convention.md             ← 커밋 메시지 작성 규칙
│   └── code-convention.md              ← C++ 코드 작성 규칙
│
├── bin/                                 ← 빌드 출력 (git 제외)
│   ├── Debug/DataPersistence.exe
│   └── Release/DataPersistence.exe
│
└── obj/                                 ← 컴파일 중간 산출물 (git 제외)
```

---

## 6. 아키텍처

### 계층 구조

```
┌─────────────────────────────────┐
│           main.cpp              │  표현 계층 — 콘솔 메뉴 UI, 사용자 입출력
└────────────────┬────────────────┘
                 │ 사용
┌────────────────▼────────────────┐
│        ItemRepository           │  비즈니스 계층 — CRUD 로직, 인메모리 캐시
│  create / findAll / findById    │  변경 발생 시 즉시 persist() 호출
│  update / remove / count        │
└────────────────┬────────────────┘
                 │ 사용
┌────────────────▼────────────────┐
│          JsonStorage            │  영속성 계층 — JSON 파일 읽기/쓰기
│       load / save               │  자체 파서 + 직렬화 내장
└────────────────┬────────────────┘
                 │ 파싱/직렬화
┌────────────────▼────────────────┐
│           Item.h                │  데이터 모델 — 순수 데이터 구조체
│  id / name / description        │
│  price / createdAt              │
└─────────────────────────────────┘
```

### 데이터 흐름

```
[실행 시작]
    │
    ▼
ItemRepository 생성자
    │── JsonStorage::load("data/items.json")
    │       └── 파일 읽기 → JSON 파싱 → vector<Item> 반환
    └── 인메모리 캐시(items_) 초기화
    │
    ▼
[사용자 입력 루프]
    │
    ├── Create ──▶ items_ 추가 ──▶ JsonStorage::save (즉시 파일 갱신)
    ├── Read   ──▶ items_ 조회 (파일 I/O 없음)
    ├── Update ──▶ items_ 수정 ──▶ JsonStorage::save (즉시 파일 갱신)
    └── Delete ──▶ items_ 제거 ──▶ JsonStorage::save (즉시 파일 갱신)
```

---

## 7. 데이터 모델

### Item 구조체 (`Item.h`)

| 필드 | 타입 | 설명 | 기본값 |
|------|------|------|--------|
| `id` | `int` | 자동 증가 식별자 (PK) | `0` |
| `name` | `std::string` | 아이템 이름 | `""` |
| `description` | `std::string` | 아이템 설명 | `""` |
| `price` | `double` | 가격 (원 단위) | `0.0` |
| `createdAt` | `std::string` | 생성 일시 (`YYYY-MM-DD HH:MM:SS`) | `""` |

### ItemRepository 공개 인터페이스

| 메서드 | 반환 타입 | 설명 |
|--------|-----------|------|
| `create(name, description, price)` | `Item` | 새 아이템 추가 후 저장 |
| `findAll()` | `std::vector<Item>` | 전체 목록 반환 |
| `findById(id)` | `std::optional<Item>` | ID 조회, 없으면 `nullopt` |
| `update(id, name, description, price)` | `bool` | 수정 성공 여부 |
| `remove(id)` | `bool` | 삭제 성공 여부 |
| `count()` | `int` | 전체 항목 수 |

---

## 8. 빌드 및 실행

### Visual Studio에서 빌드·실행

```
1. DataPersistence.sln 더블클릭
2. Ctrl+Shift+B  →  빌드
3. F5           →  디버그 실행
   Ctrl+F5      →  일반 실행
```

### 빌드 출력 경로

| 구성 | 실행 파일 경로 |
|------|----------------|
| Debug | `bin\Debug\DataPersistence.exe` |
| Release | `bin\Release\DataPersistence.exe` |

### 실행 파일 직접 실행 시 주의사항

실행 파일을 직접 실행할 경우 **실행 위치에 `data\` 폴더가 있어야 합니다.**  
Visual Studio에서 실행하면 솔루션 루트가 작업 디렉토리로 자동 설정됩니다(`vcxproj.user` 설정).

---

## 9. 기능 사용법

실행 시 다음 메뉴가 표시됩니다.

```
  ========================================
    데이터 영속성 PoC - 아이템 관리 시스템
    저장소: data/items.json
  ========================================

  1. 전체 목록 조회
  2. ID로 항목 조회
  3. 새 항목 추가  [Create]
  4. 항목 수정     [Update]
  5. 항목 삭제     [Delete]
  0. 종료
```

### 3. 새 항목 추가

```
  [새 항목 추가]
  이름     : 노트북
  설명     : 개발용 고성능 노트북
  가격(원) : 1500000
  → 추가 완료 (ID: 1, 저장됨: data/items.json)
```

### 4. 항목 수정

빈칸 입력 시 기존 값을 유지합니다. 가격은 `0` 입력 시 유지됩니다.

```
  [항목 수정]
  수정할 ID: 1
  현재 이름 : 노트북
  새 이름  (빈칸=유지):          ← Enter만 누르면 "노트북" 유지
  현재 설명 : 개발용 고성능 노트북
  새 설명  (빈칸=유지): 업무용 초경량 노트북
  현재 가격 : 1500000.00 원
  새 가격  (0=유지)   : 1200000
  → 수정 완료
```

### 5. 항목 삭제

삭제 전 확인 프롬프트가 표시됩니다.

```
  [항목 삭제]
  삭제할 ID: 1
  "노트북" 을(를) 삭제하시겠습니까? (y/N): y
  → 삭제 완료
```

---

## 10. JSON 저장 형식

`data/items.json`에 pretty-print 형식으로 저장됩니다.

```json
[
  {
    "id": 1,
    "name": "노트북",
    "description": "개발용 고성능 노트북",
    "price": 1500000.00,
    "createdAt": "2026-06-12 10:00:00"
  },
  {
    "id": 2,
    "name": "마우스",
    "description": "무선 인체공학 마우스",
    "price": 45000.00,
    "createdAt": "2026-06-12 10:05:30"
  }
]
```

### JSON 파서 지원 범위

| 항목 | 지원 여부 |
|------|-----------|
| 문자열 (`"..."`) | ✓ |
| 정수 / 실수 | ✓ |
| 이스케이프 시퀀스 (`\"`, `\\`, `\n`, `\t` 등) | ✓ |
| 객체 배열 (`[{...}, {...}]`) | ✓ |
| 중첩 객체 / 배열 | ✗ (이 PoC 범위 외) |

---

## 11. 문서

| 문서 | 설명 |
|------|------|
| [커밋 컨벤션](docs/commit-convention.md) | Conventional Commits 기반 커밋 메시지 작성 규칙 |
| [코드 컨벤션](docs/code-convention.md) | C++17 네이밍·포매팅·설계 규칙 |
| [CLAUDE.md](CLAUDE.md) | AI 협업 컨텍스트 및 빠른 참조 |
