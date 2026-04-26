#include <stdio.h>
#include <stdlib.h>

#define MEMORY_SIZE 30000

int main(int argc, char *argv[]) {
    // получаем название файла
    if (argc != 2) {
        printf("Usage: %s <.bf file>\n", argv[0]);
        return 1;
    }

    // открываем файл
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        printf("Filed to open file %s\n", argv[1]);
        return 1;
    }

    // узнаем размер файла
    fseek(file, 0, SEEK_END);
    long program_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // выделяем память под инструкции программы
    char *program = malloc(program_size + 1);
    fread(program, 1, program_size, file);
    program[program_size] = '\0';
    fclose(file);

    // инициализируем память программы и указатели на память и на инструкцию
    unsigned char memory[MEMORY_SIZE] = {0};
    unsigned char *data_ptr = memory;
    char *instruction_ptr = program;

    // проходим по инструкциям и выполняем их
    while (*instruction_ptr) {
        switch (*instruction_ptr) {
            case '>': data_ptr++; break;
            case '<': data_ptr--; break;
            case '+': (*data_ptr)++; break;
            case '-': (*data_ptr)--; break;
            case '.': putchar(*data_ptr); break; // putchar - вывод одного символа в stdout
            case ',': *data_ptr = getchar(); break; // getchar - обратное putchar

            case '[':
                // ищем ']'
                if (*data_ptr == 0) {
                    int bracket_count = 1;
                    while (bracket_count > 0) {
                        instruction_ptr++;
                        if (*instruction_ptr == '[') { bracket_count++; }
                        if (*instruction_ptr == ']') { bracket_count--; }
                        if (*instruction_ptr == '\0') {
                            printf("Error: unmatched '['\n");
                            free(program);
                            return 1;
                        }
                    }
                }
                break;

            case ']':
                // ищем '['
                if (*data_ptr != 0) {
                    int bracket_count = 1;
                    while (bracket_count > 0) {
                        instruction_ptr--;
                        if (*instruction_ptr == ']') { bracket_count++; }
                        if (*instruction_ptr == '[') { bracket_count--; }
                        if (instruction_ptr < program) {
                            printf("Error: unmatched ']'\n");
                            free(program);
                            return 1;
                        }
                    }
                }
                break;

            default: break; // все остальные символы игнорируем
        }
        instruction_ptr++;
    }

    free(program);
    return 0;
}
