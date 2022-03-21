using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UPE_MM2_ItemsAccount
{
    public class Account_ItemsBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public Account_ItemsBusinessLogicHandler()
        {
            tableName = "Account_Items";
        }
    }

    public class Account_SNOKPABusinessLogicHandler : BaseBusinessLogicHandler
    {
        public Account_SNOKPABusinessLogicHandler()
        {
            tableName = "Account_SNOKPA";
        }
    }

    public class Account_SupportItemsBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public Account_SupportItemsBusinessLogicHandler()
        {
            tableName = "Account_SupportItems";
        }
    }

    public class FAP_A50_1Account_AircraftsBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public FAP_A50_1Account_AircraftsBusinessLogicHandler()
        {
            tableName = "FAP_A50_1Account_Aircrafts";
        }
    }

    public class FAP_A50_2Account_EnginesBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public FAP_A50_2Account_EnginesBusinessLogicHandler()
        {
            tableName = "FAP_A50_2Account_Engines";
        }
    }

    public class FAP_A50_3Account_DetachItemsBusinessLogicHandler : BaseBusinessLogicHandler
    {
        public FAP_A50_3Account_DetachItemsBusinessLogicHandler()
        {
            tableName = "FAP_A50_3Account_DetachItems";
        }
    }
}
