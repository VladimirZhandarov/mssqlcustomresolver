using System.ServiceProcess;

namespace SinventCustomResolveService
{
    static class Program
    {
        /// Главная точка входа для приложения.
        static void Main()
        {
            ServiceBase[] ServicesToRun;
            ServicesToRun = new ServiceBase[]
            {
                new MainService()
            };
            ServiceBase.Run(ServicesToRun);
        }
    }
}
