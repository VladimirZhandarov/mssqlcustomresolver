using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SinventCustomResolveService;

namespace TestSinventCustomResolveService
{
    class ServiceTest : MainService
    {
        public void TestStart(string[] args)
        {
            OnStart(args);
        }

        public void TestStop()
        {
            OnStop();
        }

        public void TestPause()
        {
            OnPause();
        }

        public void TestContinue()
        {
            OnContinue();
        }
    }
}
