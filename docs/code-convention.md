# C++ 코드 컨벤션

C++17 표준 및 [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)를 기반으로 합니다.

---

## 목차

1. [파일 구성](#1-파일-구성)
2. [네이밍 규칙](#2-네이밍-규칙)
3. [포매팅](#3-포매팅)
4. [클래스 설계](#4-클래스-설계)
5. [함수·메서드](#5-함수메서드)
6. [Modern C++ 사용법](#6-modern-c-사용법)
7. [const 정확성](#7-const-정확성)
8. [에러 처리](#8-에러-처리)
9. [주석](#9-주석)
10. [금지 패턴](#10-금지-패턴)

---

## 1. 파일 구성

### 파일 확장자

| 구분 | 확장자 |
|------|--------|
| 헤더 | `.h` |
| 구현 | `.cpp` |

### 헤더 가드

`#pragma once`를 사용합니다. `#ifndef` 방식은 사용하지 않습니다.

```cpp
// ✓
#pragma once
#include <string>

// ✗
#ifndef ITEM_REPOSITORY_H
#define ITEM_REPOSITORY_H
```

### include 순서

같은 그룹 내에서는 알파벳 순으로 정렬하며, 그룹 사이에는 빈 줄을 둡니다.

```cpp
// 1. 대응하는 헤더 (*.cpp 파일에서만)
#include "ItemRepository.h"

// 2. 프로젝트 헤더
#include "Item.h"
#include "JsonStorage.h"

// 3. 시스템/표준 라이브러리 헤더
#include <algorithm>
#include <optional>
#include <string>
#include <vector>
```

### 헤더 파일 원칙

- 헤더에는 **선언만** 작성합니다. 구현은 `.cpp`에 작성합니다.
- 헤더에서 `using namespace std;`를 절대 사용하지 않습니다.
- 전방 선언(forward declaration)으로 해결 가능한 경우 불필요한 헤더 include를 피합니다.

---

## 2. 네이밍 규칙

### 요약표

| 대상                        | 규칙              | 예시                          |
|-----------------------------|-------------------|-------------------------------|
| 클래스 · 구조체 · 열거형    | `PascalCase`      | `ItemRepository`, `ParseMode` |
| 함수 · 메서드               | `camelCase`       | `findById`, `escapeString`    |
| 변수 (지역 · 매개변수)      | `camelCase`       | `filePath`, `priceStr`        |
| 멤버 변수                   | `camelCase_`      | `filePath_`, `items_`         |
| 전역 상수 · `constexpr`     | `UPPER_SNAKE_CASE`| `DATA_FILE`, `MAX_RETRY`      |
| 열거형 값                   | `PascalCase`      | `ParseMode::Strict`           |
| 네임스페이스                | `snake_case`      | `data_persistence`            |
| 매크로 (`#define`)          | `UPPER_SNAKE_CASE`| `NOMINMAX`                    |

### 세부 규칙

**클래스명**은 무엇인지를 나타내는 명사로 짓습니다.
```cpp
// ✓
class ItemRepository { ... };
struct Item { ... };

// ✗
class ManageItems { ... };   // 동사 형태
class Repo { ... };          // 불분명한 약어
```

**함수명**은 동사 또는 동사구로 짓습니다.
```cpp
// ✓
Item        create(const std::string& name, ...);
std::vector<Item> findAll() const;
bool        remove(int id);

// ✗
Item        item(const std::string& name, ...);   // 명사
std::vector<Item> items() const;                  // 명사
```

**멤버 변수**는 반드시 후행 밑줄(`_`)을 붙입니다.
```cpp
// ✓
class ItemRepository {
    std::string       filePath_;
    std::vector<Item> items_;
};

// ✗
class ItemRepository {
    std::string       m_filePath;   // 헝가리안 표기법 사용 금지
    std::vector<Item> _items;       // 선행 밑줄은 표준 예약 식별자와 충돌 가능
};
```

**bool 반환 함수**는 `is`, `has`, `can`, `should` 접두어를 사용합니다.
```cpp
// ✓
bool isEmpty()   const;
bool hasItems()  const;

// ✗
bool empty() const;   // std와 혼동 가능 (문맥에 따라 허용)
bool check() const;   // 의미 불분명
```

---

## 3. 포매팅

### 들여쓰기 및 중괄호

- **들여쓰기**: 공백 4칸. 탭 문자를 사용하지 않습니다.
- **중괄호**: K&R 스타일 — 여는 중괄호는 **같은 줄** 끝에 붙입니다.

```cpp
// ✓
class JsonStorage {
public:
    static bool save(const std::string& filePath, const std::vector<Item>& items);
};

bool JsonStorage::save(const std::string& filePath, const std::vector<Item>& items) {
    if (items.empty()) {
        return false;
    }
    for (const auto& item : items) {
        // ...
    }
    return true;
}

// ✗
bool JsonStorage::save(...)
{                           // 여는 중괄호를 다음 줄에 두지 않습니다
    if (items.empty())
        return false;       // 단일 문장도 중괄호를 생략하지 않습니다
}
```

### 단일 문장 블록

`if`, `for`, `while`의 본문이 한 줄이더라도 **반드시 중괄호**를 사용합니다.

```cpp
// ✓
if (!file.is_open()) {
    return {};
}

// ✗
if (!file.is_open())
    return {};
```

### 줄 길이

한 줄은 **120자 이내**를 권장합니다. 초과 시 적절한 위치에서 줄 바꿈합니다.

```cpp
// 매개변수가 많은 경우 — 첫 번째 매개변수 아래 정렬
bool ItemRepository::update(int id,
                             const std::string& name,
                             const std::string& description,
                             double price) {
```

### 공백

```cpp
// 연산자 양쪽에 공백
item.price = base + tax;

// 포인터·참조 기호는 타입에 붙임
const std::string& name
std::vector<Item>* ptr

// 함수 호출의 괄호 안쪽에 공백 없음
findById(id);           // ✓
findById( id );         // ✗

// 쉼표 뒤에 공백
create(name, desc, price);   // ✓
create(name,desc,price);     // ✗
```

### 헤더 선언 정렬

헤더에서 여러 멤버를 선언할 때 타입·이름을 열 기준으로 정렬합니다.

```cpp
// ✓
class ItemRepository {
    std::string       filePath_;
    std::vector<Item> items_;

    void        persist() const;
    int         nextId()  const;
    static std::string currentTimestamp();
};
```

---

## 4. 클래스 설계

### 접근 지정자 순서

`public` → `protected` → `private` 순으로 작성합니다.

```cpp
class ItemRepository {
public:
    explicit ItemRepository(const std::string& filePath);

    Item                create(...);
    std::vector<Item>   findAll() const;
    std::optional<Item> findById(int id) const;
    bool                update(...);
    bool                remove(int id);

private:
    std::string       filePath_;
    std::vector<Item> items_;

    void persist() const;
    int  nextId()  const;
};
```

### 생성자

- 단일 인자 생성자에는 **반드시 `explicit`** 를 붙입니다.
- 멤버 초기화는 **초기화 목록**을 사용합니다.

```cpp
// ✓
explicit ItemRepository(const std::string& filePath)
    : filePath_(filePath), items_(JsonStorage::load(filePath)) {}

// ✗
ItemRepository(const std::string& filePath) {
    filePath_ = filePath;                      // 대입은 초기화 목록보다 비효율적
    items_    = JsonStorage::load(filePath);
}
```

### 구조체 vs 클래스

- **데이터만 담는 집합체(aggregate)**는 `struct`를 사용합니다.
- **불변식(invariant)을 관리하거나 동작이 있는 경우**는 `class`를 사용합니다.

```cpp
// ✓ — 순수 데이터
struct Item {
    int         id        = 0;
    std::string name;
    double      price     = 0.0;
    std::string createdAt;
};

// ✓ — 불변식 + 동작
class ItemRepository {
    // 내부 items_는 항상 JSON 파일과 동기화된 상태를 유지한다는 불변식 존재
};
```

### 복사·이동 제어

컴파일러가 생성하는 기본 구현으로 충분하면 `= default`를, 명시적으로 금지할 경우 `= delete`를 사용합니다.

```cpp
// 복사 금지가 필요한 경우
class FileHandle {
public:
    FileHandle(const FileHandle&)            = delete;
    FileHandle& operator=(const FileHandle&) = delete;
    FileHandle(FileHandle&&)                 = default;
    FileHandle& operator=(FileHandle&&)      = default;
};
```

---

## 5. 함수·메서드

### 매개변수 전달 방식

| 상황 | 방식 |
|------|------|
| 읽기 전용, 복사 비쌈 | `const T&` |
| 읽기 전용, 복사 싸거나 기본형 | `T` (값) |
| 수정 필요 | `T&` |
| 소유권 이전 | `T&&` 또는 `std::unique_ptr<T>` |
| 선택적 값 | `std::optional<T>` |

```cpp
// ✓
Item create(const std::string& name,   // 복사 비쌈 → const&
            const std::string& desc,
            double price);              // 기본형 → 값

// ✗
Item create(std::string name,          // 불필요한 복사
            std::string desc,
            double& price);            // 기본형을 참조로 전달
```

### 반환값

- 여러 값 반환 시 `struct` 또는 `std::pair`/`std::tuple`을 사용합니다.
- 실패 가능한 탐색에는 `std::optional<T>`을 사용합니다.

```cpp
// ✓
std::optional<Item> findById(int id) const;

// 호출 측
if (auto item = repo.findById(42)) {
    std::cout << item->name;
}
```

### 함수 길이

함수 하나는 **화면 한 페이지(40줄 이내)**를 넘지 않도록 합니다. 길어지면 의미 단위로 분리합니다.

```cpp
// ✓ — parseItem을 별도 함수로 분리
static Item parseItem(const std::string& s, size_t& pos) { ... }

std::vector<Item> JsonStorage::load(const std::string& filePath) {
    // 파일 읽기 + parseItem 호출만 담당
}
```

---

## 6. Modern C++ 사용법

### auto

타입이 선언 문맥에서 명확히 드러날 때만 사용합니다.

```cpp
// ✓ — 우변에서 타입 명확
auto it   = std::remove_if(items_.begin(), items_.end(), pred);
auto item = repo.findById(id);   // std::optional<Item>임을 호출부에서 유추 가능

// ✗ — 타입 불분명
auto x = process();   // process가 무엇을 반환하는지 헤더 없이 알 수 없음
```

### 범위 기반 for

컨테이너 순회에는 범위 기반 for를 사용합니다.

```cpp
// ✓
for (const auto& item : items_) {
    if (item.id > maxId) maxId = item.id;
}

// ✗
for (size_t i = 0; i < items_.size(); ++i) {
    if (items_[i].id > maxId) maxId = items_[i].id;
}
```

### std::optional (C++17)

값이 없을 수 있는 반환값에는 `std::optional`을 사용하고, 포인터(`nullptr`) 또는 매직 값(`-1`)을 사용하지 않습니다.

```cpp
// ✓
std::optional<Item> findById(int id) const {
    for (const auto& item : items_) {
        if (item.id == id) return item;
    }
    return std::nullopt;
}

// ✗
Item* findById(int id);         // 소유권 모호, nullptr 체크 강제
int   findById(int id);         // -1을 매직 값으로 사용
```

### std::filesystem (C++17)

파일·디렉토리 조작에는 `<filesystem>`을 사용합니다.

```cpp
// ✓
#include <filesystem>

std::filesystem::path p(filePath);
std::error_code ec;
std::filesystem::create_directories(p.parent_path(), ec);  // 예외 없는 오버로드

// ✗
system("mkdir data");   // 이식성 없음, 보안 위험
```

### 캐스트

C 스타일 캐스트 대신 C++ 캐스트를 사용합니다.

```cpp
// ✓
static_cast<int>(items_.size())
static_cast<unsigned char>(c)

// ✗
(int)items_.size()
```

---

## 7. const 정확성

- **상태를 변경하지 않는 멤버 함수**에는 반드시 `const`를 붙입니다.
- **변경하지 않는 참조 매개변수**에는 `const T&`를 사용합니다.
- **변경하지 않는 지역 변수**에는 `const`를 붙입니다.

```cpp
// ✓
std::vector<Item> findAll() const;
int               count()   const;

bool save(const std::string& filePath,        // 읽기 전용
          const std::vector<Item>& items);    // 읽기 전용

// 지역 변수
const std::string content = ss.str();
const size_t      start   = pos;
```

---

## 8. 에러 처리

### 예외

- 복구 불가능한 프로그래밍 오류에는 `std::runtime_error`/`std::logic_error`를 사용합니다.
- 예외가 전파될 수 있는 모든 함수는 예외 안전성을 보장합니다.

```cpp
// ✓ — 파서 내부의 형식 오류
if (s[pos] != '"')
    throw std::runtime_error("JSON: '\"' 필요, pos=" + std::to_string(pos));
```

### 오류 코드

외부 시스템(파일 I/O, OS API) 호출처럼 **정상 범주 내 실패**는 `bool` 또는 `std::error_code`로 처리합니다.

```cpp
// ✓ — 파일 열기 실패는 예외 아닌 빈 결과 반환
std::vector<Item> JsonStorage::load(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) return {};   // 파일 없음은 정상 케이스
    // ...
}

// ✓ — 저장 실패는 bool 반환
bool JsonStorage::save(...) {
    std::ofstream file(filePath);
    if (!file.is_open()) return false;
    // ...
    return file.good();
}
```

---

## 9. 주석

### 언제 쓰는가

코드가 **왜** 이렇게 작성됐는지 자명하지 않을 때만 씁니다.  
**무엇을** 하는지는 잘 지어진 이름이 설명합니다.

```cpp
// ✓ — 숨겨진 제약 설명
// price는 classic locale 소수점 형식으로 직렬화한다.
// 시스템 locale이 쉼표 구분자일 경우 JSON 파싱이 실패하기 때문이다.
priceStr.imbue(std::locale::classic());

// ✗ — 코드를 그대로 설명
// filePath를 filePath_에 저장한다
filePath_ = filePath;
```

### 섹션 구분

긴 구현 파일에서 논리적 단위를 구분할 때 구분선 주석을 사용합니다.

```cpp
// ── 파서 내부 헬퍼 ─────────────────────────────

static size_t skipWs(...) { ... }
static std::string parseString(...) { ... }

// ── 공개 인터페이스 ────────────────────────────

std::vector<Item> JsonStorage::load(...) { ... }
bool JsonStorage::save(...) { ... }
```

### TODO / FIXME

임시 코드나 알려진 문제는 태그를 남깁니다.

```cpp
// TODO: 유니코드 이스케이프 시퀀스(\uXXXX) 파싱 미구현
// FIXME: 중첩 JSON 객체 입력 시 파서가 무한 루프에 빠질 수 있음
```

---

## 10. 금지 패턴

| 금지 항목 | 이유 | 대안 |
|-----------|------|------|
| `using namespace std;` (헤더) | 심볼 오염으로 예기치 않은 충돌 발생 | `std::` 명시 |
| C 스타일 캐스트 `(int)x` | 의도 불분명, 컴파일러 검사 우회 | `static_cast<int>(x)` |
| `#define`으로 상수 정의 | 타입 안전성 없음, 디버그 어려움 | `constexpr` |
| 날 포인터로 동적 할당 `new/delete` | 메모리 누수 위험 | `std::unique_ptr`, `std::make_unique` |
| `NULL` | C 스타일, 타입 안전하지 않음 | `nullptr` |
| 단일 문장 블록의 중괄호 생략 | 유지보수 중 버그 유입 경로 | 항상 `{}` 사용 |
| `goto` | 제어 흐름 추적 불가 | 구조적 제어 흐름 사용 |
| 전역 변수 (가변) | 테스트·병렬화 불가, 숨겨진 결합 | 지역 변수 또는 클래스 멤버로 격리 |
| `system()` 호출 | 이식성·보안 위험 | `std::filesystem`, WinAPI 사용 |
