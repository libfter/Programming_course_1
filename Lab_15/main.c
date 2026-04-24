#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    char name[20];
    char surname[20];
    int id;
    char level[20];
} data_t;

typedef struct {
    char value[20];
    int count;
} rle_field_t;


void print_data_table(data_t *data) {
    printf("\n%-5s | %-15s | %-15s | %-8s | %-10s\n",
           "N", "Name", "Surname", "ID", "Level");
    printf("------+-----------------+-----------------+----------+------------\n");

    for (int i = 0; i < 15; i++) {
        printf("%-5d | %-15s | %-15s | %-8d | %-10s\n",
               i + 1,
               data[i].name,
               data[i].surname,
               data[i].id,
               data[i].level);
    }
}

void search_by_id() {
    //  открываем файл и читаем его
    FILE *db_file = fopen("database.dat", "rb");
    if (db_file == NULL) {
        printf("error opening database.dat file at %d line\n", __LINE__);
        return;
    }

    data_t data[15];

    size_t read_count = fread(data, sizeof(data_t), 15, db_file);
    if (read_count != 15) {
        printf("error reading database.dat file (%d/15 records was read)\n", read_count);
        fclose(db_file);
        return;
    }

    fclose(db_file);

    //  ищем по введенному id
    int search_id;
    printf("\nвведите ID для поиска: ");
    scanf("%d", &search_id);

    int found = 0;
    for (int i = 0; i < 15; i++) {
        if (data[i].id == search_id) {
            printf("найдена запись:\n%-5d | %-15s | %-15s | %-8d | %-10s\n",
                   i+1,
                   data[i].name,
                   data[i].surname,
                   data[i].id,
                   data[i].level);
            found = 1;
        }
    }

    if (!found) {
        printf("запись не найдена\n");
    }
}

void compress() {
    FILE *db_file = fopen("database.dat", "rb");
    if (db_file == NULL) {
        printf("error opening database.dat file at %d line\n", __LINE__);
        return;
    }

    data_t data[15];
    size_t read_count = fread(data, sizeof(data_t), 15, db_file);
    if (read_count != 15) {
        printf("error reading database.dat file (%d/15 records was read)\n", read_count);
        fclose(db_file);
        return;
    }

    fclose(db_file);

    //  СЖАТИЕ LEVEL (остальные поля уникальные или повторяются слишком мало раз)
    rle_field_t rle_levels[15];
    int level_count = 0;

    for (int i = 0; i < 15; i++) {
        // LEVEL
        if (level_count == 0 || strcmp(rle_levels[level_count-1].value, data[i].level) != 0) {
            strcpy(rle_levels[level_count].value, data[i].level);
            rle_levels[level_count].count = 1;
            level_count++;
        } else {
            rle_levels[level_count-1].count++;
        }
    }

    // ЗАПИСЫВАЕМ В DATABASE.RLE
    FILE *rle_file = fopen("database.rle", "wb");
    if (rle_file == NULL) {
        printf("error opening database.rle file at %d line\n", __LINE__);
        return;
    }


    for (int i = 0; i < 15; i++) {
        // зануляем поля чтобы не было мусора
        char name[20] = {0};
        char surname[20] = {0};

        // NAMES без изменений (большая часть уникальные, сжимать смысла нет)
        fwrite(&data[i].name, sizeof(data[i].name), 1, rle_file);

        // SURNAMES без изменений (большая часть уникальные, сжимать смысла нет)
        fwrite(&data[i].surname, sizeof(data[i].surname), 1, rle_file);

        // ID без изменений (все уникальные, сжимать смысла нет)
        fwrite(&data[i].id, sizeof(int), 1, rle_file);
    }

    // LEVEL сжатый
    fwrite(&level_count, sizeof(int), 1, rle_file); // количество уникальных значений level
    fwrite(rle_levels, sizeof(rle_field_t), level_count, rle_file); // сжатые level


    fclose(rle_file);

    // СТАТИСТИКА
    int original_size = 15 * sizeof(data_t);
    int compressed_size = 15 * 20 + 15 * 20 + 15 * sizeof(int) + sizeof(int) + level_count * sizeof(rle_field_t);

    printf("\ndatabase.dat успешно сжат в database.rle\n");
    printf("Размер до: %d Б, Размер после: %d Б. Сжатие: %.1f%%\n", original_size, compressed_size, (1.0 - (double)compressed_size / original_size) * 100);

}

void decompress() {
    // ОТКРЫВАЕМ ФАЙЛ
    FILE *rle_file = fopen("database.rle", "rb");
    if (rle_file == NULL) {
        printf("error opening database.rle file at %d line\n", __LINE__);
        return;
    }

    // ЧИТАЕМ ДАННЫЕ
    data_t data[15];

    for (int i = 0; i < 15; i++) {
        fread(data[i].name, 1, 20, rle_file);
        fread(data[i].surname, 1, 20, rle_file);
        fread(&data[i].id, sizeof(int), 1, rle_file);
    }

    int level_count;
    rle_field_t rle_levels[15];
    fread(&level_count, sizeof(int), 1, rle_file);
    fread(rle_levels, sizeof(rle_field_t), level_count, rle_file);

    fclose(rle_file);

    // РАСПАКОВЫВАЕМ LEVEL
    int pos = 0;
    for (int i = 0; i < level_count; i++) {
        for (int j = 0; j < rle_levels[i].count; j++) {
            strcpy(data[pos++].level, rle_levels[i].value);
        }
    }

    // ПИШЕМ В ФАЙЛ
    FILE *out = fopen("unpacked_database.dat", "wb");
    if (out == NULL) {
        printf("error opening unpacked_database.dat file at %d line\n", __LINE__);
        return;
    }

    fwrite(data, sizeof(data_t), 15, out);
    fclose(out);

    printf("\ndatabase.rle успешно распакован в unpacked_database.dat\n");
}

void print_dat_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("error opening %s\n", filename);
        return;
    }

    data_t data[15];
    size_t read_count = fread(data, sizeof(data_t), 15, file);
    if (read_count != 15) {
        printf("error reading %s: expected 15 records, got %zu\n", filename, read_count);
        fclose(file);
        return;
    }
    fclose(file);

    printf("\n=== %s ===\n", filename);
    printf("%-20s | %-20s | %-10s | %-10s\n", "Name", "Surname", "ID", "Level");
    printf("----------------------+----------------------+------------+-----------\n");

    for (int i = 0; i < 15; i++) {
        printf("%-20s | %-20s | %-10d | %-10s\n",
               data[i].name,
               data[i].surname,
               data[i].id,
               data[i].level);
    }
}


int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("usage: %s filename\n", argv[0]);
        return 1;
    }

    //  ЧТЕНИЕ ИЗ ФАЙЛА
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("error opening '%s' file at %d line\n", argv[1], __LINE__);
        return 1;
    }

    data_t data[15];
    char buffer[255];
    for (int i = 0; i < 15; i++) {
        if (fgets(buffer, 100, file) == NULL) {
            break;
        }

        buffer[strcspn(buffer, "\n")] = 0; //удаляем \n из строки

        char *p = strtok(buffer, ";");

        if (p != NULL) {
            strcpy(data[i].name, p);
            p = strtok(NULL, ";");
        } else {
            printf("error parsing name in file at %d line\n", i+1);
            fclose(file);
            return 1;
        }
        if (p != NULL) {
            strcpy(data[i].surname, p);
            p = strtok(NULL, ";");
        } else {
            printf("error parsing surname in file at %d line\n", i+1);
            fclose(file);
            return 1;
        }
        if (p != NULL) {
            data[i].id = atoi(p);
            p = strtok(NULL, ";");
        } else {
            printf("error parsing id in file at %d line\n", i+1);
            fclose(file);
            return 1;
        }
        if (p != NULL) {
            strcpy(data[i].level, p);
        } else {
            printf("error parsing level in file at %d line\n", i+1);
            fclose(file);
            return 1;
        }
    }

    fclose(file);

    print_data_table(data);

    //  ЗАПИСЬ В DATABASE.DAT
    FILE *db_file = fopen("database.dat", "wb");
    if (db_file == NULL) {
        printf("error opening database.dat file at %d line\n", __LINE__);
        return 1;
    }

    size_t written = fwrite(data, sizeof(data_t), 15, db_file); //size_t - беззнаковый int для размеров
    if (written != 15) {
        printf("error writing to database.dat (%u/15 records written)\n", written);
        fclose(db_file);
        return 1;
    }

    printf("\nwritten 15/15 records to database.dat\n");

    fclose(db_file);

    search_by_id();

    compress();
    decompress();

    print_dat_file("database.dat");
    print_dat_file("unpacked_database.dat");

    return 0;
}
