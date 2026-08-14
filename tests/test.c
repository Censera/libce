#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <ce/ce.h>

static uint64_t hash_int(const void *value)
{
    return ce_hash_bytes(value, sizeof(int));
}

static int equal_int(const void *left, const void *right)
{
    return *(const int *)left == *(const int *)right;
}

static int compare_int(const void *left, const void *right)
{
    int a = *(const int *)left;
    int b = *(const int *)right;
    return (a > b) - (a < b);
}

static void test_core(void)
{
    size_t value;

    assert(ce_size_add(10, 20, &value) == CE_OK);
    assert(value == 30);
    assert(ce_size_mul(10, 20, &value) == CE_OK);
    assert(value == 200);
    assert(ce_size_add(SIZE_MAX, 1, &value) == CE_OVERFLOW);
    assert(ce_size_add(1, 1, NULL) == CE_INVALID_ARGUMENT);
    assert(ce_size_mul(1, 1, NULL) == CE_INVALID_ARGUMENT);
    assert(strcmp(ce_result_name(CE_OK), "ok") == 0);
    assert(strcmp(ce_result_message(CE_OVERFLOW), "size overflow") == 0);
}

static void test_view(void)
{
    static const unsigned char data[] = {1, 2, 3};
    CeView view = ce_view_make(data, sizeof(data));

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
    int output;

    assert(ce_vec_init(&vec, sizeof(int)) == CE_OK);
    assert(ce_vec_push(&vec, &(int){10}) == CE_OK);
    assert(ce_vec_push(&vec, &(int){20}) == CE_OK);
    assert(ce_vec_len(&vec) == 2);
    assert(ce_vec_elem_size(&vec) == sizeof(int));
    assert(((const int *)ce_vec_const_data(&vec))[1] == 20);
    assert(ce_vec_pop(&vec, &output) == CE_OK);
    assert(output == 20);
    assert(ce_vec_len(&vec) == 1);
    ce_vec_clear(&vec);
    assert(ce_vec_pop(&vec, &output) == CE_INVALID_DATA);
    ce_vec_free(&vec);
}

static void test_map_set(void)
{
    CeMap map;
    CeSet set;
    const void *value;
    int key = 42;
    int other = 7;
    int mapped = 99;
    int present;

    assert(ce_map_init(&map, sizeof(int), sizeof(int), hash_int, equal_int) == CE_OK);
    assert(ce_map_put(&map, &key, &mapped) == CE_OK);
    assert(ce_map_get(&map, &key, &value) == CE_OK);
    assert(*(const int *)value == 99);
    assert(ce_map_get(&map, &other, &value) == CE_NOT_FOUND);
    assert(ce_map_remove(&map, &key) == CE_OK);
    assert(ce_map_put(&map, &other, &mapped) == CE_OK);
    assert(ce_map_get(&map, &other, &value) == CE_OK);
    ce_map_free(&map);

    assert(ce_set_init(&set, sizeof(int), hash_int, equal_int) == CE_OK);
    assert(ce_set_add(&set, &key) == CE_OK);
    assert(ce_set_has(&set, &key, &present) == CE_OK);
    assert(present == 1);
    assert(ce_set_has(&set, &other, &present) == CE_OK);
    assert(present == 0);
    ce_set_free(&set);
}

static void test_queue(void)
{
    CeQueue queue;
    int value;

    assert(ce_queue_init(&queue, sizeof(int)) == CE_OK);
    assert(ce_queue_push(&queue, &(int){1}) == CE_OK);
    assert(ce_queue_push(&queue, &(int){2}) == CE_OK);
    assert(ce_queue_pop(&queue, &value) == CE_OK);
    assert(value == 1);
    assert(ce_queue_pop(&queue, &value) == CE_OK);
    assert(value == 2);
    assert(ce_queue_pop(&queue, &value) == CE_INVALID_DATA);
    ce_queue_free(&queue);
}

static void test_parse_format(void)
{
    uint64_t unsigned_value;
    int64_t signed_value;
    CeStr str;

    assert(ce_parse_u64(ce_view_make("ff", 2), &unsigned_value, 16) == CE_OK);
    assert(unsigned_value == 255);
    assert(ce_parse_i64(ce_view_make("-42", 3), &signed_value, 10) == CE_OK);
    assert(signed_value == -42);
    assert(ce_parse_i64(ce_view_make("1x", 2), &signed_value, 10) == CE_INVALID_DATA);

    assert(ce_str_init(&str, "") == CE_OK);
    assert(ce_str_format(&str, "value=%d", 42) == CE_OK);
    assert(strcmp(ce_str_data(&str), "value=42") == 0);
    assert(ce_str_append_format(&str, ", hex=%x", 255) == CE_OK);
    assert(strcmp(ce_str_data(&str), "value=42, hex=ff") == 0);
    ce_str_free(&str);
}

static void test_hash_sort(void)
{
    int values[] = {4, 1, 3, 2};
    uint64_t hash = ce_hash_bytes("abc", 3);

    assert(hash != 0);
    ce_sort(values, 4, sizeof(values[0]), compare_int);
    assert(values[0] == 1 && values[3] == 4);
    assert(*(const int *)ce_search(&(int){3}, values, 4, sizeof(values[0]), compare_int) == 3);
}

static void test_path(void)
{
    CeStr path;
    CeView basename;
    CeView extension;

    assert(ce_path_join(&path, ce_view_make("build", 5), ce_view_make("ce-test", 7)) == CE_OK);
    assert(strcmp(ce_str_data(&path), "build/ce-test") == 0);
    ce_str_free(&path);

    basename = ce_path_basename(ce_view_make("build/ce-test.txt", 17));
    extension = ce_path_extension(ce_view_make("build/ce-test.txt", 17));
    assert(basename.len == 11 && memcmp(basename.data, "ce-test.txt", 11) == 0);
    assert(extension.len == 3 && memcmp(extension.data, "txt", 3) == 0);
}

static void test_environment(void)
{
    CeStr value;
    static const char name[] = "CE_TEST_VALUE";

    assert(ce_env_set(ce_view_make(name, sizeof(name) - 1), ce_view_make("42", 2), 1) == CE_OK);
    assert(ce_env_get(ce_view_make(name, sizeof(name) - 1), &value) == CE_OK);
    assert(strcmp(ce_str_data(&value), "42") == 0);
    ce_str_free(&value);
    assert(ce_env_unset(ce_view_make(name, sizeof(name) - 1)) == CE_OK);
    assert(ce_env_get(ce_view_make(name, sizeof(name) - 1), &value) == CE_NOT_FOUND);
}

static void test_time_random(void)
{
    uint64_t now;
    CeRandom random_a;
    CeRandom random_b;

    assert(ce_time_now_ns(&now) == CE_OK);
    assert(now != 0);
    ce_random_seed(&random_a, 1234);
    ce_random_seed(&random_b, 1234);
    assert(ce_random_next(&random_a) == ce_random_next(&random_b));
}

static void test_cli(void)
{
    static const char *const argv[] = {"demo", "--name", "ce", "--verbose"};
    CeCli cli;
    CeView value;

    assert(ce_cli_init(&cli, 4, argv) == CE_OK);
    assert(ce_cli_has(&cli, ce_view_make("--verbose", 9)));
    assert(ce_cli_value(&cli, ce_view_make("--name", 6), &value) == CE_OK);
    assert(value.len == 2 && memcmp(value.data, "ce", 2) == 0);
    assert(ce_cli_value(&cli, ce_view_make("--missing", 9), &value) == CE_NOT_FOUND);
}

static void test_stream_and_file(void)
{
    static const char path[] = "build/ce-test-file";
    static const char first[] = "hello";
    static const char second[] = " world";
    CeFile file;
    CeStream stream;
    char buffer[32];
    size_t count;

    assert(ce_file_write(path, first, sizeof(first) - 1) == CE_OK);
    assert(ce_file_append(path, second, sizeof(second) - 1) == CE_OK);
    assert(ce_file_read(path, &file) == CE_OK);
    assert(file.len == sizeof(first) + sizeof(second) - 2);
    assert(memcmp(file.data, "hello world", file.len) == 0);
    ce_file_free(&file);

    assert(ce_stream_open(&stream, path, "rb") == CE_OK);
    assert(ce_stream_read(&stream, buffer, sizeof(buffer), &count) == CE_OK);
    assert(count == 11);
    assert(memcmp(buffer, "hello world", count) == 0);
    ce_stream_close(&stream);

    remove(path);
    assert(ce_file_read(path, &file) == CE_NOT_FOUND);
}

static void test_directory(void)
{
    CeDir dir;
    CeStr name;
    int done = 0;

    assert(ce_dir_open(&dir, "build") == CE_OK);
    assert(ce_dir_next(&dir, &name, &done) == CE_OK);
    if (!done)
        ce_str_free(&name);
    ce_dir_close(&dir);
}

static void test_platform_process(void)
{
    CePlatform platform = ce_platform_current();
    int status;

    assert(platform != CE_PLATFORM_UNKNOWN);
    assert(strcmp(ce_platform_name(platform), "unknown") != 0);

#ifdef _WIN32
    {
        static const char *const argv[] = {"cmd", "/c", "exit", "0", NULL};
        assert(ce_process_run("cmd", argv, &status) == CE_OK);
    }
#else
    {
        static const char *const argv[] = {"true", NULL};
        assert(ce_process_run("true", argv, &status) == CE_OK);
    }
#endif

    assert(status == 0);
}

int main(void)
{
    test_core();
    test_view();
    test_buffer();
    test_string();
    test_vector();
    test_map_set();
    test_queue();
    test_parse_format();
    test_hash_sort();
    test_path();
    test_environment();
    test_time_random();
    test_cli();
    test_stream_and_file();
    test_directory();
    test_platform_process();
    puts("ce: all tests passed");
    return 0;
}
