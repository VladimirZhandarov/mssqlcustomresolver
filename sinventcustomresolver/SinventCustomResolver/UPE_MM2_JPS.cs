using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UPE_MM2_JPS
{
    public class FaultsBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public FaultsBusinessLogicHandler()
        {
            tableName = "Faults";
        }
    }

    public class FlightPrepareControlListBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public FlightPrepareControlListBusinessLogicHandler()
        {
            tableName = "FlightPrepareControlList";
        }
    }

    public class ImportFaultsBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public ImportFaultsBusinessLogicHandler()
        {
            tableName = "ImportFaults";
        }
    }

    public class ImportFaultsDetailBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public ImportFaultsDetailBusinessLogicHandler()
        {
            tableName = "ImportFaultsDetail";
        }
    }

    public class ImportXLSBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public ImportXLSBusinessLogicHandler()
        {
            tableName = "ImportXLS";
        }
    }

    public class ImportXLSDetailBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public ImportXLSDetailBusinessLogicHandler()
        {
            tableName = "ImportXLSDetail";
        }
    }

    public class JobTypesBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public JobTypesBusinessLogicHandler()
        {
            tableName = "JobTypes";
        }
    }

    public class JPSBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public JPSBusinessLogicHandler()
        {
            tableName = "JPS";
        }
    }

    public class KUNBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public KUNBusinessLogicHandler()
        {
            tableName = "KUN";
        }
    }

    public class KUN2BusinessLogicHandler : BaseBusinessLogicHandler
    {
        public KUN2BusinessLogicHandler()
        {
            tableName = "KUN2";
        }
    }

    public class PilotRemarksBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public PilotRemarksBusinessLogicHandler()
        {
            tableName = "PilotRemarks";
        }
    }

    public class PlanJobsBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public PlanJobsBusinessLogicHandler()
        {
            tableName = "PlanJobs";
        }
    }

    public class PrepareRoutesBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public PrepareRoutesBusinessLogicHandler()
        {
            tableName = "PrepareRoutes";
        }
    }

    public class ReliabilityFormulasBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public ReliabilityFormulasBusinessLogicHandler()
        {
            tableName = "ReliabilityFormulas";
        }
    }

    public class SOK_ReportDetailsBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public SOK_ReportDetailsBusinessLogicHandler()
        {
            tableName = "SOK_ReportDetails";
        }
    }

    public class SOK_ReportsBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public SOK_ReportsBusinessLogicHandler()
        {
            tableName = "SOK_Reports";
        }
    }

    public class TypedPlanJobsBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public TypedPlanJobsBusinessLogicHandler()
        {
            tableName = "TypedPlanJobs";
        }
    }
}
