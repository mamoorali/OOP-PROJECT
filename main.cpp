#include "ChatEngine.h"
#include "FileStorage.h"
#include "DatabaseStorage.h"

int main()
{
    FileStorage* storage = new FileStorage("data/");

    {
        ChatEngine engine(storage);
        engine.start();
    }

    delete storage;
    return 0;
}
