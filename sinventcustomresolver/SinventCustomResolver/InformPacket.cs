
namespace SinventCustomResolver
{
    /** Пакет с информацией о ходе репликации.
     */
    public class InformPacket
    {
        public int errorCode = 0;       ///< Код ошибки (0 - ошибки нет)
        public string message = null;   ///< Текст сообщения
    }
}
