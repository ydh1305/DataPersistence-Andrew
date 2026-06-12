# 커밋 컨벤션

[Conventional Commits 1.0.0](https://www.conventionalcommits.org/ko/v1.0.0/) 명세를 기반으로 합니다.

---

## 기본 형식

```
<type>(<scope>): <subject>

[body]

[footer]
```

- **type**, **subject**는 필수입니다.
- **scope**, **body**, **footer**는 선택입니다.
- 제목 줄은 **72자 이내**로 작성합니다.
- 제목 끝에 마침표(`.`)를 붙이지 않습니다.

---

## Type 목록

| Type       | 사용 시점                                              |
|------------|--------------------------------------------------------|
| `feat`     | 새로운 기능 추가                                       |
| `fix`      | 버그 수정                                              |
| `docs`     | 문서 변경 (코드 변경 없음)                             |
| `style`    | 포맷, 세미콜론 등 코드 의미에 영향 없는 변경           |
| `refactor` | 버그 수정·기능 추가가 아닌 코드 구조 변경              |
| `test`     | 테스트 추가 또는 수정                                  |
| `build`    | 빌드 시스템·외부 의존성 변경 (vcxproj, CMake 등)       |
| `ci`       | CI/CD 설정 변경                                        |
| `chore`    | 소스·테스트에 영향 없는 기타 변경 (.gitignore 등)      |
| `revert`   | 이전 커밋 되돌리기                                     |

---

## Scope (선택)

변경이 영향을 미치는 모듈·레이어를 소문자로 작성합니다.

| Scope        | 대상                              |
|--------------|-----------------------------------|
| `storage`    | JsonStorage (파일 I/O, 파서)      |
| `repository` | ItemRepository (CRUD 로직)        |
| `model`      | Item 데이터 구조체                |
| `ui`         | main.cpp 콘솔 UI                  |
| `build`      | vcxproj, sln 등 빌드 설정         |
| `docs`       | 문서 파일                         |

---

## Subject 작성 규칙

- **명령형·현재 시제**로 작성합니다 ("추가한다" ✓ / "추가했다" ✗)
- 첫 글자를 대문자로 쓰지 않습니다 (영문의 경우)
- 제목만으로 **무엇을**, **왜** 변경했는지 알 수 있어야 합니다

---

## Body (선택)

- 제목 줄과 빈 줄 하나로 구분합니다.
- **무엇을 왜** 변경했는지 설명합니다. **어떻게**는 코드로 충분합니다.
- 줄 길이는 **72자 이내**를 권장합니다.

---

## Footer (선택)

- **Breaking Change**: `BREAKING CHANGE: <설명>` 으로 시작합니다.
- **이슈 참조**: `Closes #123`, `Refs #456` 형식으로 작성합니다.

---

## 예시

### feat — 기능 추가
```
feat(repository): 가격 범위로 항목 검색 기능 추가

최소/최대 가격을 인자로 받아 범위 내 항목 목록을 반환한다.
ItemRepository::findByPriceRange(double min, double max)
```

### fix — 버그 수정
```
fix(storage): JSON 직렬화 시 소수점 locale 오류 수정

시스템 locale이 쉼표 소수점(예: de_DE)일 때 price 필드가
"1500000,00"으로 출력되어 파싱 실패하는 문제를 수정한다.
classic locale을 명시적으로 지정하여 항상 점(.) 구분자를 사용한다.
```

### docs — 문서 변경
```
docs: 커밋 컨벤션 문서 추가
```

### build — 빌드 설정 변경
```
build: PrecompiledHeader를 NotUsing으로 설정

MSVC 기본값(Use)이 pch.h를 요구해 C1010 에러가 발생하므로
PCH를 명시적으로 비활성화한다.
```

### refactor — 리팩토링
```
refactor(storage): parseItem을 전용 헬퍼 함수로 분리

load() 내부의 파싱 로직이 길어져 가독성이 떨어지므로
static parseItem()으로 추출한다. 동작 변경 없음.
```

### revert — 되돌리기
```
revert: feat(repository): 가격 범위 검색 기능 추가

This reverts commit a1b2c3d.
```

---

## Breaking Change 예시

```
feat(model): Item에 category 필드 추가

기존 JSON 파일과 호환되지 않으므로 마이그레이션이 필요하다.

BREAKING CHANGE: items.json 스키마 변경. 기존 파일에 "category" 키가
없으면 로드 시 기본값("")으로 처리된다.
```

---

## 빠른 참조

```
feat(scope): 새 기능 추가
fix(scope): 버그 수정
docs: 문서만 변경
style: 포맷 변경
refactor(scope): 리팩토링
build: 빌드 설정 변경
chore: 기타 변경
```
