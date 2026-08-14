#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <ce/ce.h>

static void test_core(void)
{
    size_t value;

    assert(ce_size_add(10, 20, &value) == CE_OK);
    assert(value == 30);
    assert(ce_size_mul(10, 20, &value) == CE_OK);
    assert(value == 200);
    assert(ce_size_add((size_t)-1, 1, &value) == CE_OVERFLOW);
    assert(ce_size_add(1, 1, NULL) == CE_INVALID_ARGUMENT);
    assert(ce_size_mul(1, 1, NULL) == CE_INVALID_ARGUMENT);
}

static void test_view(void)
{
    static const unsigned char data[] = {1, 2, 3};
    CeView view = {data, sizeof(data)};

    assert(view.data == data);
    assert(view.len == sizeof(data));
}

static void test_buffer(void)
{
    CeBuf buf;
    const char text[] = "hello";

    assert(ce_buf_init(&buf) == CE_OK);
    assert(ce_buf_append(&buf, text, sizeof(text) - 1) == CE_OK);
    assert(ce_buf_append_byte(&buf, '!') == CE_OK);
    assert(ce_buf_len(&buf) == 6);
    assert(memcmp(ce_buf_const_data(&buf), "hello!", 6) == 0);
    assert(ce_buf_append(NULL, text, 1) == CE_INVALID_ARGUMENT);
    assert(ce_buf_append(&buf, NULL, 1) == CE_INVALID_ARGUMENT);
    assert(ce_buf_append(&buf, NULL, 0) == CE_OK);
    ce_buf_clear(&buf);
    assert(ce_buf_len(&buf) == 0);
    ce_buf_free(&buf);
}

static void test_string(void)
{
    CeStr str;

    assert(ce_str_init(&str, NULL) == CE_INVALID_ARGUMENT);
    assert(ce_str_init(&str, "hello") == CE_OK);
    assert(ce_str_append(&str, " world") == CE_OK);
    assert(ce_str_append_char(&str, '!') == CE_OK);
    assert(strcmp(ce_str_data(&str), "hello world!") == 0);
    assert(ce_str_len(&str) == strlen("hello world!"));
    assert(ce_str_append(NULL, "x") == CE_INVALID_ARGUMENT);
    assert(ce_str_append(&str, NULL) == CE_INVALID_ARGUMENT);
    ce_str_clear(&str);
    assert(strcmp(ce_str_data(&str), "") == 0);
    ce_str_free(&str);
}

static void test_vector(void)
{
    CeVec vec;
    int value;
    int output;

    assert(ce_vec_init(&vec, sizeof(value)) == CE_OK);
    assert(ce_vec_push(&vec, &(int){10}) == CE_OK);
    assert(ce_vec_push(&vec, &(int){20}) == CE_OK);
    assert(ce_vec_len(&vec) == 2);
    assert(ce_vec_elem_size(&vec) == sizeof(value));

    memcpy(&value, (const unsigned char *)ce_vec_const_data(&vec) + sizeof(value), sizeof(value));
    assert(value == 20);

    assert(ce_vec_pop(&vec, &output) == CE_OK);
    assert(output == 20);
    assert(ce_vec_len(&vec) == 1);
    ce_vec_clear(&vec);
    assert(ce_vec_len(&vec) == 0);
    assert(ce_vec_pop(&vec, &output) == CE_INVALID_DATA);

    ce_vec_free(&vec);
    assert(ce_vec_init(NULL, sizeof(value)) == CE_INVALID_ARGUMENT);
    assert(ce_vec_init(&vec, 0) == CE_INVALID_ARGUMENT);
}

static void test_file(void)
{
    static const char path[] = "build/ce-test-file";
    static const char first[] = "hello";
    static const char second[] = " world";
    CeFile file;

    assert(ce_file_write(path, first, sizeof(first) - 1) == CE_OK);
    assert(ce_file_append(path, second, sizeof(second) - 1) == CE_OK);
    assert(ce_file_read(path, &file) == CE_OK);
    assert(file.len == sizeof(first) + sizeof(second) - 2);
    assert(memcmp(file.data, "hello world", file.len) == 0);
    ce_file_free(&file);
    remove(path);
    assert(ce_file_read("build/no-such-file", &file) == CE_NOT_FOUND);
}

int main(void)
{
    test_core();
    test_view();
    test_buffer();
    test_string();
    test_vector();
    test_file();
    puts("ce: all tests passed");
    return 0;
}
