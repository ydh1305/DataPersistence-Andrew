#define NOMINMAX
#include <windows.h>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <limits>
#include "ItemRepository.h"

static const std::string DATA_FILE = "data/items.json";

// ──────────────────────────────────────────────
// 출력 헬퍼
// ──────────────────────────────────────────────

static void sep() {
    std::cout << "  ----------------------------------------\n";
}

static void printHeader() {
    std::cout << "\n  ========================================\n";
    std::cout << "    데이터 영속성 PoC - 아이템 관리 시스템\n";
    std::cout << "    저장소: " << DATA_FILE << "\n";
    std::cout << "  ========================================\n";
}

static void printMenu() {
    std::cout << "\n"
              << "  1. 전체 목록 조회\n"
              << "  2. ID로 항목 조회\n"
              << "  3. 새 항목 추가  [Create]\n"
              << "  4. 항목 수정     [Update]\n"
              << "  5. 항목 삭제     [Delete]\n"
              << "  0. 종료\n"
              << "\n  선택: ";
}

static void printItem(const Item& item) {
    sep();
    std::cout << "  ID       : " << item.id          << "\n"
              << "  이름     : " << item.name         << "\n"
              << "  설명     : " << item.description  << "\n"
              << "  가격     : " << std::fixed << std::setprecision(2) << item.price << " 원\n"
              << "  생성일시 : " << item.createdAt    << "\n";
    sep();
}

// ──────────────────────────────────────────────
// 입력 헬퍼
// ──────────────────────────────────────────────

static void flushInput() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line;
}

static int readInt(const std::string& prompt) {
    std::cout << prompt;
    int val = 0;
    while (!(std::cin >> val)) {
        std::cin.clear();
        flushInput();
        std::cout << "  숫자를 입력하세요: ";
    }
    flushInput();
    return val;
}

static double readDouble(const std::string& prompt) {
    std::cout << prompt;
    double val = 0.0;
    while (!(std::cin >> val)) {
        std::cin.clear();
        flushInput();
        std::cout << "  숫자를 입력하세요: ";
    }
    flushInput();
    return val;
}

// ──────────────────────────────────────────────
// 메뉴 액션
// ──────────────────────────────────────────────

static void listAll(ItemRepository& repo) {
    auto items = repo.findAll();
    std::cout << "\n  [전체 목록] 총 " << items.size() << "개\n";
    if (items.empty()) {
        std::cout << "  (데이터 없음)\n";
        return;
    }
    sep();
    std::cout << std::left
              << "  " << std::setw(5)  << "ID"
              <<         std::setw(22) << "이름"
              <<         std::setw(14) << "가격(원)"
              <<         "생성일시\n";
    sep();
    for (const auto& item : items) {
        std::ostringstream priceStr;
        priceStr << std::fixed << std::setprecision(2) << item.price;
        std::cout << "  " << std::left
                  << std::setw(5)  << item.id
                  << std::setw(22) << item.name
                  << std::setw(14) << priceStr.str()
                  << item.createdAt << "\n";
    }
    sep();
}

static void cmdFindById(ItemRepository& repo) {
    int id = readInt("  조회할 ID: ");
    auto result = repo.findById(id);
    if (!result) {
        std::cout << "  ID " << id << " 을(를) 찾을 수 없습니다.\n";
        return;
    }
    std::cout << "\n  [조회 결과]\n";
    printItem(*result);
}

static void cmdCreate(ItemRepository& repo) {
    std::cout << "\n  [새 항목 추가]\n";
    std::string name = readLine("  이름     : ");
    std::string desc = readLine("  설명     : ");
    double      price = readDouble("  가격(원) : ");

    auto item = repo.create(name, desc, price);
    std::cout << "  → 추가 완료 (ID: " << item.id << ", 저장됨: " << DATA_FILE << ")\n";
}

static void cmdUpdate(ItemRepository& repo) {
    std::cout << "\n  [항목 수정]\n";
    int id = readInt("  수정할 ID: ");

    auto existing = repo.findById(id);
    if (!existing) {
        std::cout << "  ID " << id << " 을(를) 찾을 수 없습니다.\n";
        return;
    }

    std::cout << "  현재 이름 : " << existing->name << "\n";
    std::string name = readLine("  새 이름  (빈칸=유지): ");
    if (name.empty()) name = existing->name;

    std::cout << "  현재 설명 : " << existing->description << "\n";
    std::string desc = readLine("  새 설명  (빈칸=유지): ");
    if (desc.empty()) desc = existing->description;

    std::cout << "  현재 가격 : " << std::fixed << std::setprecision(2) << existing->price << " 원\n";
    double price = readDouble("  새 가격  (0=유지)   : ");
    if (price == 0.0) price = existing->price;

    if (repo.update(id, name, desc, price))
        std::cout << "  → 수정 완료\n";
    else
        std::cout << "  수정 실패\n";
}

static void cmdDelete(ItemRepository& repo) {
    std::cout << "\n  [항목 삭제]\n";
    int id = readInt("  삭제할 ID: ");

    auto existing = repo.findById(id);
    if (!existing) {
        std::cout << "  ID " << id << " 을(를) 찾을 수 없습니다.\n";
        return;
    }

    std::cout << "  \"" << existing->name << "\" 을(를) 삭제하시겠습니까? (y/N): ";
    char confirm = 'N';
    std::cin >> confirm;
    flushInput();

    if (confirm == 'y' || confirm == 'Y') {
        repo.remove(id);
        std::cout << "  → 삭제 완료\n";
    } else {
        std::cout << "  취소되었습니다.\n";
    }
}

// ──────────────────────────────────────────────
// 진입점
// ──────────────────────────────────────────────

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    ItemRepository repo(DATA_FILE);

    printHeader();
    std::cout << "  현재 " << repo.count() << "개의 항목이 로드되었습니다.\n";

    int choice = -1;
    while (choice != 0) {
        printMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            flushInput();
            std::cout << "  올바른 번호를 입력하세요.\n";
            continue;
        }
        flushInput();

        switch (choice) {
            case 1: listAll(repo);      break;
            case 2: cmdFindById(repo);  break;
            case 3: cmdCreate(repo);    break;
            case 4: cmdUpdate(repo);    break;
            case 5: cmdDelete(repo);    break;
            case 0: std::cout << "\n  종료합니다.\n"; break;
            default: std::cout << "  잘못된 선택입니다. 0~5 중에서 입력하세요.\n"; break;
        }
    }
    return 0;
}
