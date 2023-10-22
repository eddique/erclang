#ifndef erclang_debug_h
#define erclang_debug_h
#include "chunk.h"
void disassembleChunk(Chunk* chunk, const char* name);
int disassembleInstruction(Chunk* chunk, int offset);
#endif