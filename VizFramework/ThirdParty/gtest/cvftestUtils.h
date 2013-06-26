


namespace cvftest {


//==================================================================================================
//
// Static helper class for unit tests
//
//==================================================================================================
class Utils
{
public:
    static cvf::String getTestDataDir(const cvf::String& unitTestFolder)
    {
        // Expects environment variable on this form 
        //   Linux:  export CVF_UTEST_TESTDATA="../../../Tests/TestData/"
        std::string testPath = getEnvironmentVar("CVF_UTEST_TEST_DATA_DIR");
        if (testPath.empty())
        {
            std::string exe = getMyExecutablePath();
            testPath = exe.substr(0, exe.find(unitTestFolder.toStdString())) + std::string("TestData/");
        }

        return testPath;
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

