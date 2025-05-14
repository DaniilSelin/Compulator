#include "Structs.h"
#include "LexemeReader.h"
#include "GenOPS.h"

int main() {
    Context ctx;
    std::string filePath = "G:/Coding/test/Compuler/include.txt";

    std::string inputString = loadTextFile(filePath);
    std::vector<Lexeme> prog = lexemeReader(ctx, inputString);

    std::cout << prog << std::endl;

    std::vector<Literal> temp = genOPS(prog);
    return 0;
}
