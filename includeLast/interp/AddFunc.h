inline std::string loadTextFile(const std::string& filePath) {
    std::ifstream in(filePath);
    if (!in) {
        std::cout << "Не открывается файл: " << filePath << std::endl;
        return std::string();
    }
    std::string output((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    output += "#";
    return output;
}