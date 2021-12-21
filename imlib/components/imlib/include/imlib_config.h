#ifndef IMLIB_CONFIG_H
#define IMLIB_CONFIG_H
#ifdef __cplusplus
extern "C"
{
#endif


#ifndef LOG_EN
#define LOG_EN
#define LOG_PRINT(fmt, ...) do{\
		log_printf("[info] "fmt"\t\t[info: %s:%d] [%s]\n", ##__VA_ARGS__, strrchr(__FILE__, '/') + 1, __LINE__, __FUNCTION__);\
}while(0);
#endif


// #define IMLIB_ENABLE_FLOOD_FILL

#define IMLIB_ENABLE_YUV_LAB_FUNC







#ifdef __cplusplus
}
#endif






#endif