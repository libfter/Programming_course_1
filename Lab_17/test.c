#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <cjson/cJSON.h>

void test_parse_simple_object(void **state) {
    char *json = "{\"x\":10, \"y\":20}";

    cJSON *parsed = cJSON_Parse(json);
    assert_non_null(parsed);
    assert_true(cJSON_IsObject(parsed));

    cJSON *x = cJSON_GetObjectItem(parsed, "x");
    assert_non_null(x);
    assert_true(cJSON_IsNumber(x));
    assert_int_equal(x->valueint, 10);

    cJSON *y = cJSON_GetObjectItem(parsed, "y");
    assert_non_null(y);
    assert_true(cJSON_IsNumber(y));
    assert_int_equal(y->valueint, 20);

    cJSON_Delete(parsed);
}

void test_create_object(void **state) {
    cJSON *json = cJSON_CreateObject();
    assert_non_null(json);

    cJSON_AddNumberToObject(json, "age", 25);
    cJSON_AddStringToObject(json, "name", "John");

    cJSON *age = cJSON_GetObjectItem(json, "age");
    assert_non_null(age);
    assert_int_equal(age->valueint, 25);

    cJSON *name = cJSON_GetObjectItem(json, "name");
    assert_non_null(name);
    assert_string_equal(name->valuestring, "John");

    cJSON_Delete(json);
}

void test_parse_invalid(void **state) {
    char *json = "{128}";
    cJSON *parsed = cJSON_Parse(json);
    assert_null(parsed);

    json = "{abc}";
    parsed = cJSON_Parse(json);
    assert_null(parsed);
}

void test_parse_array(void **state) {
    const char *json = "[10, 20, 30]";
    cJSON *parsed = cJSON_Parse(json);
    assert_non_null(parsed);

    assert_true(cJSON_IsArray(parsed));
    assert_int_equal(cJSON_GetArraySize(parsed), 3);

    /* Проверяем каждый элемент */
    cJSON *item = cJSON_GetArrayItem(parsed, 0);
    assert_non_null(item);
    assert_int_equal(item->valueint, 10);

    item = cJSON_GetArrayItem(parsed, 1);
    assert_int_equal(item->valueint, 20);

    item = cJSON_GetArrayItem(parsed, 2);
    assert_int_equal(item->valueint, 30);

    cJSON_Delete(parsed);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_parse_simple_object),
        cmocka_unit_test(test_create_object),
        cmocka_unit_test(test_parse_invalid),
        cmocka_unit_test(test_parse_array)
    };

    cmocka_set_message_output(CM_OUTPUT_TAP);

    return cmocka_run_group_tests(tests, NULL, NULL);
}
