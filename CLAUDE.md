# DataPersistence-Andrew — PoC

JSON 파일 기반 데이터 영속성 처리 PoC (C++ / Visual Studio 2022)

## 개요

콘솔 기반 아이템 관리 시스템으로, JSON 파일에 데이터를 저장하고 불러오는 구조를 구현합니다.  
외부 라이브러리 없이 자체 JSON 파서로 동작하며, 완전한 CRUD 기능을 제공합니다.

## 기술 스택

- **언어**: C++17
- **IDE**: Visual Studio 2022 (v143 toolset)
- **플랫폼**: Windows x64
- **JSON 처리**: 자체 구현 (외부 의존성 없음)

## 프로젝트 구조

```
DataPersistence-Andrew/
├── CLAUDE.md
├── DataPersistence.sln          # VS 솔루션 파일
├── DataPersistence/
│   ├── DataPersistence.vcxproj
│   ├── DataPersistence.vcxproj.filters
│   ├── DataPersistence.vcxproj.user  # 디버그 작업 디렉토리 설정
│   └── src/
│       ├── main.cpp             # 진입점, 콘솔 메뉴 UI
│       ├── Item.h               # 데이터 모델 (id, name, description, price, createdAt)
│       ├── JsonStorage.h/.cpp   # JSON 파일 읽기/쓰기 (파서 내장)
│       └── ItemRepository.h/.cpp # CRUD 비즈니스 로직
└── data/
    └── items.json               # 런타임에 자동 생성되는 데이터 파일
```

## 빌드 방법

1. `DataPersistence.sln` 더블클릭으로 Visual Studio 2022 실행
2. 솔루션 탐색기에서 `DataPersistence` 프로젝트 우클릭 → **빌드**
3. 또는 단축키 `Ctrl+Shift+B`

## 실행 방법

- **Visual Studio에서**: `F5` (디버그) 또는 `Ctrl+F5` (디버그 없이 실행)
- **실행 파일 직접**: `bin\Debug\DataPersistence.exe`
  - 이 경우 실행 위치에 `data\` 폴더가 존재해야 합니다.

> 디버그 작업 디렉토리는 솔루션 루트(`$(SolutionDir)`)로 설정되어 있어  
> `data/items.json` 경로가 자동으로 올바르게 지정됩니다.

## 데이터 모델

| 필드        | 타입   | 설명          |
|-------------|--------|---------------|
| id          | int    | 자동 증가 PK  |
| name        | string | 아이템 이름   |
| description | string | 아이템 설명   |
| price       | double | 가격 (원)     |
| createdAt   | string | 생성 일시     |

## JSON 형식 예시

```json
[
  {
    "id": 1,
    "name": "노트북",
    "description": "개발용 고성능 노트북",
    "price": 1500000.00,
    "createdAt": "2026-06-12 10:00:00"
  }
]
```

## 아키텍처

```
main.cpp
  └─ ItemRepository      (CRUD 로직, 인메모리 캐시 + 영속성 관리)
       └─ JsonStorage     (JSON 파일 I/O, 자체 파서/직렬화)
            └─ Item.h     (데이터 구조체)
```

- **ItemRepository**: 아이템 컬렉션을 메모리에 유지하며, 변경 시 즉시 파일에 저장
- **JsonStorage**: 파일 읽기/쓰기 및 JSON 파싱/직렬화 담당
- 외부 JSON 라이브러리 미사용 → 의존성 없이 바로 빌드 가능

## CRUD 기능

| 번호 | 기능          | 설명                          |
|------|---------------|-------------------------------|
| 1    | 전체 목록 조회 | 저장된 모든 아이템 표 형식 출력 |
| 2    | ID로 조회      | 특정 ID 아이템 상세 조회       |
| 3    | 추가 (Create)  | 새 아이템 입력 후 저장         |
| 4    | 수정 (Update)  | ID 지정, 빈칸 입력 시 기존 값 유지 |
| 5    | 삭제 (Delete)  | 삭제 전 확인 프롬프트          |

## 기여 가이드

| 문서 | 내용 |
|------|------|
| [README](README.md) | 요구사항, 소스 구조, 빌드·실행 방법 전체 안내 |
| [커밋 컨벤션](docs/commit-convention.md) | Conventional Commits 기반 type·scope·subject 규칙 및 예시 |
| [코드 컨벤션](docs/code-convention.md) | C++17 네이밍·포매팅·클래스 설계·Modern C++ 사용 규칙 |
