using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UPE_MM2_Administration2
{
    public class AdmActivityBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public AdmActivityBusinessLogicHandler()
        {
            tableName = "admActivity";
        }
    }

    public class AdmActivityModuleBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public AdmActivityModuleBusinessLogicHandler()
        {
            tableName = "admActivityModule";
        }
    }

    public class AdmActivityUsingHistoryBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public AdmActivityUsingHistoryBusinessLogicHandler()
        {
            tableName = "admActivityUsingHistory";
        }
    }

    public class AdmAircraftAccessBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public AdmAircraftAccessBusinessLogicHandler()
        {
            tableName = "admAircraftAccess";
        }
    }

    public class AdmRoleBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public AdmRoleBusinessLogicHandler()
        {
            tableName = "admRole";
        }
    }

    public class AdmRoleAccessBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public AdmRoleAccessBusinessLogicHandler()
        {
            tableName = "admRoleAccess";
        }
    }

    public class AdmSystemEventAccountBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public AdmSystemEventAccountBusinessLogicHandler()
        {
            tableName = "admSystemEventAccount";
        }
    }

    public class AdmUserAccessBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public AdmUserAccessBusinessLogicHandler()
        {
            tableName = "admUserAccess";
        }
    }

    public class AdmUserInRoleBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public AdmUserInRoleBusinessLogicHandler()
        {
            tableName = "admUserInRole";
        }
    }

    public class AdmUserLogsBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public AdmUserLogsBusinessLogicHandler()
        {
            tableName = "admUserLogs";
        }
    }

    public class AdmUserPasswordBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public AdmUserPasswordBusinessLogicHandler()
        {
            tableName = "admUserPassword";
        }
    }

    public class AdmVacancyRolesBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public AdmVacancyRolesBusinessLogicHandler()
        {
            tableName = "admVacancyRoles";
        }
    }
}
