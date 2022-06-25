
void log_message(char* level, char* file, int line, char* message);

#define INFO(MESSAGE) log_message("[INFO] ", __FILE__, __LINE__, MESSAGE)
#define WARN(MESSAGE) log_message("[WARN] ", __FILE__, __LINE__, MESSAGE)
#define ERROR(MESSAGE) log_message("[ERROR]", __FILE__, __LINE__, MESSAGE)
