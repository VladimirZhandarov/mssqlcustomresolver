using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UPE_MM2_Reglament2
{
    public class CA_TblTasksBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public CA_TblTasksBusinessLogicHandler()
        {
            tableName = "CA_TblTasks";
        }
    }

    public class CB_TblSubTasksBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public CB_TblSubTasksBusinessLogicHandler()
        {
            tableName = "CB_TblSubTasks";
        }
    }

    public class CC_TblSuccSubTasksBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public CC_TblSuccSubTasksBusinessLogicHandler()
        {
            tableName = "CC_TblSuccSubTasks";
        }
    }

    public class CE_TblCommentsBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public CE_TblCommentsBusinessLogicHandler()
        {
            tableName = "CE_TblComments";
        }
    }

    public class CG_TblSupportEqBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public CG_TblSupportEqBusinessLogicHandler()
        {
            tableName = "CG_TblSupportEq";
        }
    }

    public class CH_TblManualsBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public CH_TblManualsBusinessLogicHandler()
        {
            tableName = "CH_TblManuals";
        }
    }

    public class CI_TblAuxBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public CI_TblAuxBusinessLogicHandler()
        {
            tableName = "CI_TblAux";
        }
    }

    public class CI_TblSparesBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public CI_TblSparesBusinessLogicHandler()
        {
            tableName = "CI_TblSpares";
        }
    }

    public class CK_TblPeopleFunctionsBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public CK_TblPeopleFunctionsBusinessLogicHandler()
        {
            tableName = "CK_TblPeopleFunctions";
        }
    }

    public class CL_TblWarningsBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public CL_TblWarningsBusinessLogicHandler()
        {
            tableName = "CL_TblWarnings";
        }
    }

    public class TblReglamentJobTypesMaintenanceBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public TblReglamentJobTypesMaintenanceBusinessLogicHandler()
        {
            tableName = "TblReglamentJobTypesMaintenance";
        }
    }

    public class TblReglamentLinksBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public TblReglamentLinksBusinessLogicHandler()
        {
            tableName = "TblReglamentLinks";
        }
    }

    public class TblReglamentPunktLinksWithMaintenanceBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public TblReglamentPunktLinksWithMaintenanceBusinessLogicHandler()
        {
            tableName = "TblReglamentPunktLinksWithMaintenance";
        }
    }

    public class TblReglamentsBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public TblReglamentsBusinessLogicHandler()
        {
            tableName = "TblReglaments";
        }
    }

    public class TblReglamentStringsBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public TblReglamentStringsBusinessLogicHandler()
        {
            tableName = "TblReglamentStrings";
        }
    }
}
