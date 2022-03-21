using System.Collections.Generic;

namespace SinventCustomResolveService
{
    public class ReplicationObjectDataSet
    {
        public string dbName;
        public List<string> tables;
        public List<string> storedProcedures;
        public List<string> userFunctions;
    }
    public class ReplicationObjectPacket
    {
        public enum ReplicationObjectResult
        {
            UndefineReplicationObjectResult = 0,   ///< Не определено
            AcceptReplicationObjectResult,         ///< Ок
            RejectReplicationObjectResult,         ///< Отмена
        }; ///< Ответ от сервиса клиенту

        public string username;
        public string password;
        public List<ReplicationObjectDataSet> dbNames;
        public string errorMessage;                  ///< Сообщение
        public ReplicationObjectResult result;  ///< Ответ от сервиса клиенту
    }
}
