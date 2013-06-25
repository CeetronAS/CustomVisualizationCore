


namespace gctest {


//==================================================================================================
//
// Static helper class for GLview Components unit tests
//
//==================================================================================================
class Utils
{
public:
    static std::string getTestDataDirAscii()
    {
        // Expects environment variable on this form 
        //   Linux:  export GC_UTEST_TESTDATA="../../../Tests/TestData/"
        std::string testPath = getEnvironmentVar("GC_UTEST_TEST_DATA_DIR");
        if (testPath.empty())
        {
            std::string exe = getMyExecutablePath();
#ifdef WIN32
            testPath = exe.substr(0, exe.find("GLviewComponents\\dev\\")) + std::string("GLviewComponents\\dev\\Tests\\TestData\\");
#else
            testPath = exe.substr(0, exe.find("GLviewComponents/dev/")) + std::string("GLviewComponents/dev/Tests/TestData/");
#endif
        }

        return testPath;
    }


    static std::wstring getTestDataDir()
    {
        std::string testPath = getTestDataDirAscii();

        std::wstring wTestPath;
        wTestPath.assign(testPath.begin(), testPath.end());

        return wTestPath;
    }


private:
    static std::string getEnvironmentVar(const char* name)
    {        
        const char* env = testing::internal::posix::GetEnv(name);
        if (env)
        {
            return env;
        }
        else
        {
            return std::string();
        }
    }


    static std::string getMyExecutablePath()
    {
#ifdef WIN32
        std::string exe = std::string(testing::internal::GetArgvs()[0]);
#else
        std::string dir = std::string(testing::internal::FilePath::GetCurrentDir().ToString());
        std::string exe = dir + std::string("/") + std::string(testing::internal::GetArgvs()[0]);
#endif
        return exe;
    }
};


}

