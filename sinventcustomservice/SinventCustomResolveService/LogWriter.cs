using System.IO;

namespace SinventCustomResolveService
{
    /** Ведение журнала.
     */
    public class LogWriter
    {
        string m_filePath;  ///< Имя файла журнала
        private static readonly object locker = new object(); ///< Mutex для добавления записей в файл журнала

        public enum LogLevel
        {
            Normal = 0,
            Trace
        }
        LogLevel m_logLevel;    ///< Уровень журналирования

        /** Конструктор.
         * @param filePath: имя файла журнала.
         * @param logLevel: уровень журналирования.
         */
        public LogWriter(string filePath, LogLevel logLevel)
        {
            m_filePath = filePath;
            m_logLevel = logLevel;
        }

        /** Добавить сообщение в журнал.
         * @param message: сообщение.
         */
        public void logNormalMessage(string message)
        {
            logMessage(message, LogLevel.Normal);
        }

        /** Добавить TRACE сообщение в журнал.
         * @param message: сообщение.
         */
        public void logTraceMessage(string message)
        {
            logMessage(message, LogLevel.Trace);
        }

        /** Добавить сообщение в журнал.
         * @param message: сообщение.
         * @param logLevel: уровень журналирования.
         */
        public void logMessage(string message, LogLevel logLevel)
        {
            if (logLevel <= m_logLevel)
            {
                lock (locker)
                {
                    StreamWriter logStreamWriter;
                    logStreamWriter = File.AppendText(m_filePath);
                    logStreamWriter.WriteLine(System.DateTime.Now.ToString("[MM\\/dd\\/yyyy h\\:mm tt] ") + message);
                    logStreamWriter.Close();
                }
            }
        }
    }
}
