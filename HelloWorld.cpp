#include <cstdint>
#include <Windows.h>

struct STDOutHandleGetStdHandleArgumentReturner {
    DWORD getGetStdHandleArgument() {
        return STD_OUTPUT_HANDLE;
    }
};

struct STDOutHandleReturner {
    HANDLE getHandle() {
        return GetStdHandle(STDOutHandleGetStdHandleArgumentReturner().getGetStdHandleArgument());
    }
};

struct Character {
private:
    uint8_t value;
public:
    Character(): Character(0) {}
    Character(uint8_t value): value(value) {}
    uint8_t getValue() {
        return value;
    }
    void setValue(uint8_t value) {
        this->value = value;
    }
};

struct CharacterPrinter {
private:
    Character character;
    STDOutHandleReturner stdOutHandleReturner;
public:
    CharacterPrinter(Character character, STDOutHandleReturner stdOutHandleReturner): character(character), stdOutHandleReturner(stdOutHandleReturner) {}
    Character getCharacter() {
        return character;
    }
    void setCharacter(Character character) {
        this->character = character;
    }
    void print() {
        uint8_t data = character.getValue();
        WriteConsoleA(stdOutHandleReturner.getHandle(), &data, 1, nullptr, 0);
    }
};

struct String {
private:
    Character* characters;
    size_t size;
public:
    String(uint8_t* data, size_t size) {
        characters = new Character[0];
        this->size = 0;
        for (size_t i = 0; i < size; i++) {
            this->push_back(Character(data[i]));
        }
        if (size != this->size) abort();
    }

    ~String() {
        delete[] characters;
    }

    void push_back(Character character) {
        size_t oldSize = size;
        size_t newSize = oldSize + 1;
        Character* newCharacters = new Character[newSize];
        for (size_t i = 0; i < oldSize; i++) {
            newCharacters[i] = characters[i];
        }
        newCharacters[oldSize] = character;
        delete[] characters;
        this->size = newSize;
        this->characters = newCharacters;
    }

    void print() {
        for (size_t i = 0; i < size; i++) {
            STDOutHandleReturner stdOutHandleReturner;
            CharacterPrinter characterPrinter(characters[i], stdOutHandleReturner);
            characterPrinter.print();
        }
    }
};

struct CStringStrLen {
    size_t strlen(uint8_t* data) {
        size_t i = 0;
        while (data[i] != '\0') {
            i++;
        }
        return i;
    }
};

struct HelloWorldStringFactory {
private:
    uint8_t actualData[15] = "Hello, world!\0";
public:

    String constructString() {
        CStringStrLen strLen;
        size_t size = strLen.strlen(actualData);
        return String(actualData, size);
    }
};

int main()
{
    HelloWorldStringFactory helloWorldStringFactory;
    String string = helloWorldStringFactory.constructString();
    string.print();
    return 0;
}