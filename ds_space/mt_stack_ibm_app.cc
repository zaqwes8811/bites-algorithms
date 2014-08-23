// Передает пакеты и prodecer thread to consimer
// Так похоже правильнее было реализовать поток данных
//   из интерфейса в базу данных
//
//
// Deamonds:
//   Task parallelize
//   каждый пакет должн быть обработан
//   io-thread развязан от потока обработки
//   correct concurent read form database
//
// Info:
// http://web.stanford.edu/class/cs140/cgi-bin/lecture.php?topic=process
// http://programmers.stackexchange.com/questions/206962/should-multi-threading-be-used-for-tasks-which-does-not-involve-io-operation
//
// coupled - Queue
// https://www.quantnet.com/threads/c-multithreading-in-boost.10028/
// http://stackoverflow.com/questions/10139251/shared-queue-in-c
//
// concurent queue
// http://stackoverflow.com/questions/16275112/boost-c-lock-free-queue-vs-shared-queue
// http://www.alexey-martynov.pp.ru/index.php?data=articles&article=mt-queue
// http://www.justsoftwaresolutions.co.uk/threading/implementing-a-thread-safe-queue-using-condition-variables.html
// http://stackoverflow.com/questions/6959023/c-producer-consumer-queue-with-very-fast-and-reliable-handover
// http://blog.ruslans.com/2013/08/introduction-to-high-level.html
//
// http://channel9.msdn.com/Events/GoingNative/2013/Cpp-Seasoning
