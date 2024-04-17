#ifndef UNITY_CONFIG_H
#define UNITY_CONFIG_H

#ifdef __cplusplus
    extern "C"{
#endif

void Unity_outputStart(void);
void Unity_outputChar(int c);
void Unity_outputFlush(void);
void Unity_outputComplete(void);

#define UNITY_OUTPUT_START() Unity_outputStart()
#define UNITY_OUTPUT_CHAR(c) Unity_outputChar(c)
#define UNITY_OUTPUT_FLUSH() Unity_outputFlush()
#define UNITY_OUTPUT_COMPLETE() Unity_outputComplete()

#ifdef __cplusplus
    }
#endif

#endif