#include "../src/common.hpp"
#include "../src/loader.hpp"
#include "../src/executor.hpp"

int run_test(const char* filePath) {
    try {
        const YVM::Bytecode::Data &code = YVM::Loader::load(filePath);
        YVM::Executor::execute(code);
    } catch (YVM::Exception::YvmException &exception) {
        return exception.errorCode;
    } catch (std::exception &exception) {
        ERROR_LOG("VM crashed due to unhandled exception: %s\n", exception.what());
        return -1;
    }
    return 0;
}

int main() {
    // TODO: Aggiungere RntObj come esito dei per controllare il risultato
    // TODO: Execute deve ritornare l'ultimo valore come risultato dell'esecuzione (?)
    // TODO: Dividere i test in sessione e stampare un output decente
    std::vector<const char*> filesToTest = {
            ".\\tests\\00_helloworld.yrin"
    };
    for (const auto& file : filesToTest) {
        run_test(file);
    }
    return 0;
}
