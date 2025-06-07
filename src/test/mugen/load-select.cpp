#include <iostream>
#include "util/init.h"
#include "util/configuration.h"
#include "util/message-queue.h"
#include "util/file-system.h"
#include "mugen/character-select.h"
#include "mugen/exception.h"
#include "mugen/parse-cache.h"
#include "util/timedifference.h"
#include "util/graphics/bitmap.h"
#include "util/debug.h"

/*
#include <sstream>
#include <unistd.h>
*/

using namespace std;

/*
static int getPid(){
    return getpid();
}

static void runPmap(int pid){
    ostringstream out;
    out << "pmap " << pid;
    system(out.str().c_str());
}

static void showMemory(){
    runPmap(getPid());
}
*/

static int load(const char * path){
    // showMemory();
    for (int i = 0; i < 1; i++){
        try{
            TimeDifference diff;
            diff.startTime();
            Global::debug(0) << "Loading " << path << endl;
            //Mugen::CharacterSelect select(Storage::instance().find(Filesystem::RelativePath(path)), Mugen::Player1, Mugen::Arcade);
            Mugen::CharacterSelect select(Storage::instance().find(Filesystem::RelativePath(path)));
            select.init();
            diff.endTime();
            Global::debug(0, "test") << diff.printTime("Success! Took") << endl;
            /*
               int * x = new int[1 << 21];
               delete[] x;
               */
        } catch (const MugenException & e){
            Global::debug(0, "test") << "Test failure!: " << e.getReason() << endl;
            return 1;
        } catch (const Filesystem::NotFound & e){
            Global::debug(0, "test") << "Test failure! Couldn't find a file: " << e.getTrace() << endl;
            return 1;
        }
    }
    return 0;
    // showMemory();
}

int main(int argc, char ** argv){
    Global::InitConditions conditions;
    conditions.graphics = Global::InitConditions::Disabled;
    Global::init(conditions);

    Global::setDebug(1);
    Mugen::ParseCache cache;
    Util::Thread::initializeLock(&MessageQueue::messageLock);

    int die = 0;
    if (argc < 2){
        die = load("mugen/data/system.def");
    } else {
        die = load(argv[1]);
    }

    return die;
}
