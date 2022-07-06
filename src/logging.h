
void log_message(char* level, char* file, int line, char* format, ...);

#define INFO(MESSAGE, ...) log_message("[INFO] ", __FILE__, __LINE__, MESSAGE, ## __VA_ARGS__)
#define WARN(MESSAGE, ...) log_message("[WARN] ", __FILE__, __LINE__, MESSAGE, ## __VA_ARGS__)
#define ERROR(MESSAGE, ...) log_message("[ERROR]", __FILE__, __LINE__, MESSAGE, ## __VA_ARGS__)
