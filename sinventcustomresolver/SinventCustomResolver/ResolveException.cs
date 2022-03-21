using System;
using System.Runtime.Serialization;

namespace SinventCustomResolver
{
    /** Исключение при процессе репликации.
     */
    public class ResolveException : ApplicationException
    {
        /** Конструктор.
         */
        public ResolveException()
        {
        
        }

        /** Конструктор.
         * @param message: сообщение
         */
        public ResolveException(string message) : base(message)
        {
        
        }

        /** Конструктор.
         * @param message: сообщение
         * @param inner: внутреннее исключение
         */
        public ResolveException(string message, Exception inner) : base(message, inner)
        {
        
        }

        protected ResolveException(SerializationInfo info, StreamingContext context) : base(info, context)
        {
        
        }
    }
}
