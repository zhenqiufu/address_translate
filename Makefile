CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = 

# 目标文件和依赖关系
TARGET1 = power2hashing
TARGET2 = non_power2hashing
OBJS1 = power2hashing.o
OBJS2 = non_power2hashing.o
DEPS = address_translate.h

# 默认目标
all: $(TARGET1) $(TARGET2)

# 编译 file1.c 生成 file1 可执行文件
$(TARGET1): $(OBJS1)
	$(CC) $(LDFLAGS) -o $@ $^

# 编译 file2.c 生成 file2 可执行文件
$(TARGET2): $(OBJS2)
	$(CC) $(LDFLAGS) -o $@ $^

# 生成目标文件 file1.o
file1.o: file1.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $^ file1.c

# 生成目标文件 file2.o
file2.o: file2.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $^

# 清理编译生成的文件
clean:
	rm -f $(TARGET1) $(TARGET2) $(OBJS1) $(OBJS2)
