/* OO_MPI_IO.h (Version 2) declares C++ templates that use MPI_IO:
 *  - ParallelReader to read data from a binary file in parallel.
 *  - ParallelWriter to write data to a binary file in parallel.
 *
 * The template allows you to pass a type-parameter indicating
 *   the type of the data in the file.
 *
 * @author: Joel C. Adams, for CS 374 at Calvin University.
 *
 * Version History:
 *   Version 1 (pure MPI), Fall 2023, assumes that the program
 *     using OO_MPI_IO calls MPI_Init() and MPI_Finalize().
 *   Version 2 (for OpenMP), Spring 2025, modifies 
 *     - the constructors to (if necessary) call MPI_Init_thread() 
 *     - the destructor to (if necessary) call MPI_Finalize() 
 *     so that a multithreaded program need not call them explicitly.
 *     Also adds method ParallelReader::getChunkPlus(int extras) for 
 *      reading a chunk plus extras items from the next PE's chunk
 *      (useful for search problems where the target spans chunk boundaries).
 *
 * Note: OO_MPI_IO uses 'PEs' (processing elements) as a synonym
 *        for threads or processes.
 */

#ifndef OO_MPI_IO
#define OO_MPI_IO

#include <mpi.h>                     // C MPI
#include <omp.h>                     // C OpenMP
#include <string>                    // C++ string 
#include <cmath>                     // ceil()
#include <vector>                    // C++ vector
#include <climits>

/********************************************************************
 * OO_MPI_IO_Base is a base class for C++ templates that use
 *  MPI_IO to read/write binary data from/to files in parallel.
 *
 * Its subclasses are ParallelReader and ParallelWriter.
 ********************************************************************/

template<class ItemType> 
class OO_MPI_IO_Base {
public:
  OO_MPI_IO_Base(const std::string& fileName, 
                   int openMode, MPI_Datatype mpiType,
                   int id, int numPEs);
  virtual ~OO_MPI_IO_Base();
 
  int getID() const                { return myID; }
  int getNumPEs() const            { return myNumPEs; }
  long getItemSize() const         { return myItemSize; }
  std::string getFileName() const  { return myFileName; }
  MPI_File& getFileHandle()        { return myFileHandle; }
  MPI_Datatype getMPIType() const  { return myMPIType; }
  long getNumItemsInFile() const   { return myNumItemsInFile; }
  long getChunkSize() const        { return myChunkSize; }
  long getFirstItemOffset() const  { return myFirstItemOffset; }
  long getFirstByteOffset() const  { return myFirstByteOffset; }
  long getFileSize() const         { return myFileSize; }

  void close()                     { MPI_File_close(&myFileHandle); }

protected:
  void setID(int newID);
  void setNumPEs(int newNumPEs);

  void setNumItemsInFile(long numItemsInFile) {
        myNumItemsInFile = numItemsInFile;
  }
  void setFileSize(MPI_Offset fileSize) {
        myFileSize = fileSize;
  }
  void setChunkSize(long chunkSize) {
        myChunkSize = chunkSize;
  }
  void setFirstItemOffset(long firstItemOffset) {
        myFirstItemOffset = firstItemOffset; 
  }
  void setFirstByteOffset(long firstByteOffset) {
        myFirstByteOffset = firstByteOffset;
  }

private:
  int          myID;                  // thread id or MPI rank
  int          myNumPEs;              // num threads or MPI processes
  int          myItemSize;            // size of 1 Item
  std::string  myFileName;            // file being opened
  MPI_Datatype myMPIType;             // the MPI equiv of ItemType
  MPI_File     myFileHandle;          // MPI handle for file 
  bool         myFinalizeFlag;        // true iff MPI_Init not called

  // these attributes are unknown until read or write is called
  long         myNumItemsInFile;      // total Items to be read
  MPI_Offset   myFileSize;            // size of file in bytes
  long         myChunkSize;           // size of my chunk to read
  long         myFirstItemOffset;     // offset of my chunk (Item #)
  long         myFirstByteOffset;     // offset of my chunk (byte #)
};

/* Utility to check the return-values of MPI-IO function calls
 * @param: result, an int
 * Precondition:  result is the return-value from the last MPI-IO call.
 * Postcondition: If result is anything other than MPI_SUCCESS::
 *                 the string associated with result has been printed to stderr
 *                 && the program has been terminated abnormally.
 */
void checkResult(int result) {
  if (result != MPI_SUCCESS) {
    char errorString[1024] = {'\0'};
    int  errorStringLength = -1;
    int  errorClass = -1;

    MPI_Error_class(result, &errorClass);
    MPI_Error_string(errorClass, errorString, &errorStringLength);
    fprintf(stderr, "\nMPI Error: '%s'\n\n", errorString);

    MPI_Abort(MPI_COMM_WORLD, result);
  }
}

/* OO_MPI_IO_BASE constructor
 * @param: fileName, a string
 * @param: openMode, an int
 * @param: mpiType, an MPI_Datatype value
 * @param: id, an int
 * @param: numPEs, an int
 * Precondition: fileName is the name of a file containing
 *                binary-format values of type ItemType
 *           &&  openMode is a valid MPI file-opening mode
 *           &&  mpiType is the MPI equivalent of ItemType
 *           &&  id is a thread id or MPI process rank
 *           &&  numPEs is the number of threads or processes.
 * Postcondition: if MPI_Init() has not already been called 
 *                 then MPI_Init_thread() has been called
 *           &&   the file has been opened for parallel IO
 *                 as specified by openMode
 *           &&   each instance variable have been initialized
 *                 as appropriate for this PE.
 * Note: It would be cleaner to pass mpiType as a template parameter
 *         but doing so produces errors (at least for OpenMPI and clang),
 *         so this is a hack-ey workaround.
 */
template <class ItemType>
OO_MPI_IO_Base<ItemType>::
OO_MPI_IO_Base(const std::string& fileName, int openMode, MPI_Datatype mpiType,
                int id, int numPEs) {
   myFileName = fileName;
   myMPIType = mpiType;
   myItemSize = sizeof(ItemType);
   if (id >= 0) { 
      myID = id; 
   } else { 
      fprintf(stderr, "\nOO_MPI_IO_Base(): id must be non-negative\n\n");
      exit(1);
   } 
   if (numPEs > 0) { 
      myNumPEs = numPEs;
   } else { 
      fprintf(stderr, "\nOO_MPI_IO_Base(): numPEs must be positive\n\n");
      exit(1);
   } 

   // set other attributes to defaults for now
   myNumItemsInFile = 0;
   myFileSize = 0;
   myChunkSize = 0;
   myFirstItemOffset = 0;
   myFirstByteOffset = 0;
   myFinalizeFlag = false;

   // for OpenMP: the main thread needs to call MPI_Init_thread()
   int mpiInitFlag = 0;
   MPI_Initialized(&mpiInitFlag);                    // was MPI_Init() called?
   if (!mpiInitFlag && omp_get_thread_num() == 0) {  // if not:
      int modeProvided = 0;                          //  assume OpenMP prog and
      int initResult = MPI_Init_thread(0, 0,         //  call MPI_Init_thread()
                                MPI_THREAD_MULTIPLE, //  so any thread can make
                                &modeProvided);      //  MPI calls
      checkResult(initResult);
      myFinalizeFlag = true;                         // set flag for destructor
   }
                                                     // make faster threads
   #pragma omp barrier                               // wait until
                                                     // MPI_Init_thread() done

   int openResult = MPI_File_open( MPI_COMM_WORLD,    // communicator
                                    fileName.c_str(), // name of file
                                    openMode,         // mode parameter
                                    MPI_INFO_NULL,    // skip this
                                    &myFileHandle );  // MPI handle
   checkResult(openResult);
}

/* parameter-checking setter methods for id, numPEs
 */
template <class ItemType>
void OO_MPI_IO_Base<ItemType>::setID(int newID) {
   if (newID < 0) {
      fprintf(stderr, "\nOO_MPI_IO_Base::setID(): bad id (%d)\n\n", newID);
      exit(1);
   }
   myID = newID;
}

template <class ItemType>
void OO_MPI_IO_Base<ItemType>::setNumPEs(int newNumPEs) {
   if (newNumPEs < 0) {
      fprintf(stderr, "\nOO_MPI_IO_Base::setNumPEs(): bad numPEs (%d)\n\n",
                     newNumPEs);
      exit(1);
   }
   myNumPEs = newNumPEs;
}

/* OO_MPI_IO_BASE destructor cleans up at object's end-of-life
 * Postcondition: the shared file has been closed 
 *             && if we called MPI_Init_thread(),
 *                  we have called MPI_Finalize().
 */
template <class ItemType>
OO_MPI_IO_Base<ItemType>::~OO_MPI_IO_Base() {
   MPI_File_close(&myFileHandle);          // close shared file
   #pragma omp barrier                     // ensure all threads done
   if (myFinalizeFlag &&                   // if MPI_Init() was not called 
       omp_get_thread_num() == 0) {        //  and we are the main thread,
      MPI_Finalize();                      //   then we need to finalize
   }
 }

/* Calculate the start and stop values for this PE's 
 *  contiguous chunk of a set of loop-iterations, 0..REPS-1,
 *  so that PEs' chunk-sizes are equal (or nearly so).
 *
 * @param: id, an int containing this PE's id (thread id or MPI rank)
 * @param: numPEs, an int containing the number of PEs
 * @param: REPS, a const unsigned containing the for loop's iteration total
 * Precondition: id == this thread's id or MPI process's rank
 *            && numPEs == the number of threads or MPI processes
 *            && REPS == the total number of 0-based loop iterations needed
 *            && numPEs <= REPS 
 *            && REPS < 2^32
 * @param: start, a long reference through which the 
 *          starting value of this PE's chunk should be returned
 * @param: stop, a long reference through which the
 *          stopping value of this PE's chunk should be returned
 * Postcondition: start == this PE's first iteration value 
 *             && stop == this PE's last iteration value + 1.
 */
void getChunkStartStopValues(int id, int numPEs, const unsigned REPS,
                              long& start, long& stop)
{
   // check precondition before proceeding
   if ((unsigned)numPEs > REPS) {
      if (id == 0) {
         printf("\n*** Number of PEs (%u) exceeds REPS (%u)\n",
                 numPEs, REPS);
         printf("*** Please run using PEs less than or equal to %u\n\n", REPS);
      }
      MPI_Finalize();
      exit(1);
   }

   // compute the chunk size that works in many cases
   unsigned chunkSize1 = (long)ceil(((double)REPS) / numPEs);
   unsigned begin = id * chunkSize1;
   unsigned end = begin + chunkSize1;
   // see if there are any leftover iterations
   unsigned remainder = REPS % numPEs;
   // If remainder == 0, chunkSize1 = chunk-size for all PEs;
   // If remainder != 0, chunkSize1 = chunk-size for p_0..p_remainder-1
   //   but for PEs p_remainder..p_numPEs-1
   //   recompute begin and end using a smaller-by-1 chunk size, chunkSize2.
   if (remainder > 0 && (unsigned)id >= remainder) {
     unsigned chunkSize2 = chunkSize1 - 1;
     unsigned remainderBase = remainder * chunkSize1;
     unsigned peOffset = (id-remainder) * chunkSize2;
     begin = remainderBase + peOffset;
     end = begin + chunkSize2;
   } 
   // pass back this PE's begin and end values via start and stop
   start = begin;
   stop = end;
} 

/*******************************************************************
 * The ParallelReader template provides an abstraction to hide the
 *  details of MPI-IO parallel input.
 *
 * It uses OO_MPI_IO_Base as its superclass.
 ******************************************************************/

template<class ItemType> 
class ParallelReader : public OO_MPI_IO_Base<ItemType> {
public:
  ParallelReader(const std::string& fileName, MPI_Datatype mpiType,
                  int id, int numPEs);
  std::vector<ItemType> readChunk();
  std::vector<ItemType> readChunkPlus(unsigned numExtras);
};

/* ParallelReader constructor
 * @param: fileName, a string
 * @param: mpiType, an MPI_Datatype value
 * @param: id, an int
 * @param: numPEs, an int
 * Precondition: fileName is the name of a file containing
 *                binary-format values of type ItemType
 *           &&  mpiType is the MPI_Datatype that corresonds to ItemType
 *               (e.g., MPI_DOUBLE for double)
 *           &&  id is a thread id or MPI process rank
 *           &&  numPEs is the number of threads or processes.
 * Postcondition: the file has been opened for parallel input
 *           &&  each instance variable have been initialized
 *                as appropriate for this PE using the file's info.
 */
template <class ItemType>
ParallelReader<ItemType>::
ParallelReader(const std::string& fileName, MPI_Datatype mpiType,
                int id, int numPEs)
: OO_MPI_IO_Base<ItemType>(fileName, MPI_MODE_RDONLY, mpiType, id, numPEs)
{ }

/* method to read a chunk from the file (in its entirety).
 * Return: a vector containing the values of this PE's chunk.
 * Note: vector was chosen as the return-type because it
 *        uses contiguous memory and provides a move-constructor.
 */
template <class ItemType>
std::vector<ItemType> 
ParallelReader<ItemType>::readChunk() {
   // Note: We could compute the following attributes in the constructor, 
   //  but do them here for symmetry with ParallelWriter
   MPI_Offset fileSize;
   MPI_File_get_size(OO_MPI_IO_Base<ItemType>::getFileHandle(), &fileSize);
   OO_MPI_IO_Base<ItemType>::setFileSize(fileSize);
   if (std::is_same<ItemType, char>::value) {         // if ItemType is char
      --fileSize;                                     // ignore EOF char
   }
   OO_MPI_IO_Base<ItemType>::setNumItemsInFile( fileSize / OO_MPI_IO_Base<ItemType>::getItemSize() );

   long start = 0, stop = 0;
   getChunkStartStopValues(OO_MPI_IO_Base<ItemType>::getID(), 
                           OO_MPI_IO_Base<ItemType>::getNumPEs(),
                           OO_MPI_IO_Base<ItemType>::getNumItemsInFile(),
                           start, stop);
   OO_MPI_IO_Base<ItemType>::setChunkSize(stop - start);
   OO_MPI_IO_Base<ItemType>::setFirstItemOffset(start);
   OO_MPI_IO_Base<ItemType>::setFirstByteOffset(
                              start * OO_MPI_IO_Base<ItemType>::getItemSize());

   MPI_Status status;
   unsigned long itemsRead = 0;
   unsigned long itemsToRead = OO_MPI_IO_Base<ItemType>::getChunkSize();
   std::vector<ItemType> v( itemsToRead );
   int readResult = 0;
   // handle very large files where chunkSize > INT_MAX
   while (itemsToRead > INT_MAX) {
      readResult = MPI_File_read_at(OO_MPI_IO_Base<ItemType>::getFileHandle(), 
                      OO_MPI_IO_Base<ItemType>::getFirstByteOffset()+itemsRead, 
                      v.data()+itemsRead,
                      INT_MAX, 
                      OO_MPI_IO_Base<ItemType>::getMPIType(), 
                      &status);
      checkResult(OO_MPI_IO_Base<ItemType>::getID(), readResult);
      itemsRead += INT_MAX;
      itemsToRead -= INT_MAX;
   }
   // read in remaining Items (or if itemsToRead <= INT_MAX initially)
   readResult = MPI_File_read_at(OO_MPI_IO_Base<ItemType>::getFileHandle(), 
                    OO_MPI_IO_Base<ItemType>::getFirstByteOffset()+itemsRead, 
                    v.data()+itemsRead,
                    itemsToRead, 
                    OO_MPI_IO_Base<ItemType>::getMPIType(), 
                    &status);
   checkResult(OO_MPI_IO_Base<ItemType>::getID(), readResult);

   return v;
}

/* method to read a chunk from the file (in its entirety)
 *  plus a few items from the next PE's chunk.
 *  This version of the method is useful for search problems where
 *  the sought-after value straddles the boundary between two chunks.
 * @param: numExtras, an unsigned.
 * Precondition: numExtras is the number of additional Items to be read
 *                beyond the end of this PE's chunk.
 * Return: a vector containing the values of this PE's chunk
 *          plus numExtras values of the next PE's chunk
 *          for all PEs except the last one.
 *
 * Note: Giving numExtras a default parameter value would let us eliminate
 *        the other readChunk() method and adhere to the DRY principle,
 *        but defining each separately lets us more clearly explain
 *        the difference between the two versions.
 */
template <class ItemType>
std::vector<ItemType>
ParallelReader<ItemType>::readChunkPlus(unsigned numExtras) {
   // Note: We could compute the following attributes in the constructor,
   //  but do them here for symmetry with ParallelWriter
   MPI_Offset fileSize;
   MPI_File_get_size(OO_MPI_IO_Base<ItemType>::getFileHandle(), &fileSize);
   OO_MPI_IO_Base<ItemType>::setFileSize(fileSize);
   if (std::is_same<ItemType, char>::value) {         // if ItemType is char
      --fileSize;                                     // ignore EOF char
   }
   OO_MPI_IO_Base<ItemType>::setNumItemsInFile( fileSize / OO_MPI_IO_Base<ItemType>::getItemSize() );

   long numItemsInFile = OO_MPI_IO_Base<ItemType>::getNumItemsInFile();
   long start = 0, stop = 0;
   int id = OO_MPI_IO_Base<ItemType>::getID();
   int numPEs = OO_MPI_IO_Base<ItemType>::getNumPEs();
   getChunkStartStopValues(id, numPEs, numItemsInFile, start, stop);
   if (id < numPEs-1) {
      stop += numExtras;
   }
   if (stop > numItemsInFile ) {
      stop = numItemsInFile;
   }
   OO_MPI_IO_Base<ItemType>::setChunkSize(stop - start);
   OO_MPI_IO_Base<ItemType>::setFirstItemOffset(start);
   OO_MPI_IO_Base<ItemType>::setFirstByteOffset(start * OO_MPI_IO_Base<ItemType>::getItemSize());

   MPI_Status status;
   unsigned long itemsRead = 0;
   unsigned long itemsToRead = OO_MPI_IO_Base<ItemType>::getChunkSize();
   std::vector<ItemType> v( itemsToRead );
   int readResult = 0;
   // handle very large files where chunkSize > INT_MAX
   while (itemsToRead > INT_MAX) {
      readResult = MPI_File_read_at(OO_MPI_IO_Base<ItemType>::getFileHandle(), 
                      OO_MPI_IO_Base<ItemType>::getFirstByteOffset()+itemsRead, 
                      v.data()+itemsRead,
                      INT_MAX, 
                      OO_MPI_IO_Base<ItemType>::getMPIType(), 
                      &status);
      checkResult(readResult);
      itemsRead += INT_MAX;
      itemsToRead -= INT_MAX;
   }
   // read in remaining Items (or if itemsToRead <= INT_MAX initially)
   readResult = MPI_File_read_at(OO_MPI_IO_Base<ItemType>::getFileHandle(), 
                    OO_MPI_IO_Base<ItemType>::getFirstByteOffset()+itemsRead, 
                    v.data()+itemsRead,
                    itemsToRead, 
                    OO_MPI_IO_Base<ItemType>::getMPIType(), 
                    &status);
   checkResult(readResult);

   return v;
}

/*******************************************************************
 * The ParallelWriter template provides an abstraction to hide the
 *  details of MPI-IO parallel output.
 *
 * It uses OO_MPI_IO_Base as its superclass.
 ******************************************************************/

template<class ItemType> 
class ParallelWriter : public OO_MPI_IO_Base<ItemType> {
public:
  ParallelWriter(const std::string& fileName, MPI_Datatype mpiType,
                  int id, int numPEs); 
  void writeChunk(const std::vector<ItemType>& v);
private:
};

/* ParallelWriter constructor
 * @param: fileName, a string
 * @param: id, an int
 * @param: numPEs, an int
 * Precondition: fileName is the name of an output file 
 *                to which binary-format values 
 *                of type ItemType are to be written.
 *             && mpiType is the MPI equivalent of ItemType
 *                 (e.g., MPI_DOUBLE for double)
 *             && id is a thread id or MPI process rank
 *             && numPEs is the number of threads or MPI processes.
 * Postcondition: the file has been opened for parallel output
 *             &&  each instance variable have been initialized
 *                  as appropriate for this PE using id, numPEs,
 *                  and size info from the file.
 */
template <class ItemType>
ParallelWriter<ItemType>::
ParallelWriter(const std::string& fileName, MPI_Datatype mpiType,
                int id, int numPEs)
: OO_MPI_IO_Base<ItemType>(fileName,
                            MPI_MODE_WRONLY | MPI_MODE_CREATE,  
                            mpiType, id, numPEs)
{}  // no instance variables, so no local initializations


/* method to write this PE's chunk to the file
 * @param: v, a vector of Items.
 * Precondition: v contains the Items to be output to a file.
 * Postcondition: v's values have been written to the file
 *         at the appropriate offsets for this PE.
 * Note: It would be cleaner to pass mpiType as a template parameter
 *         but doing so produces errors (at least for OpenMPI and clang),
 *         so this is a hack-ey workaround.
 *       Could instead pass it as a parameter to the constructor...
 */
template <class ItemType>
void ParallelWriter<ItemType>::writeChunk(const std::vector<ItemType>& v) {
   MPI_File_set_size(OO_MPI_IO_Base<ItemType>::getFileHandle(), 0); // truncate

   long chunkSize = v.size();
   OO_MPI_IO_Base<ItemType>::setChunkSize(chunkSize);
   
   long totalItems;
   MPI_Allreduce(&chunkSize, &totalItems, 1,   // find total #
                   MPI_LONG, MPI_SUM, MPI_COMM_WORLD);  //  of Items

   OO_MPI_IO_Base<ItemType>::setNumItemsInFile(totalItems);
   int itemSize = sizeof(ItemType);
   long totalBytes = totalItems * itemSize;
   OO_MPI_IO_Base<ItemType>::setFileSize(totalBytes); 
   long start = 0, stop = 0;
   getChunkStartStopValues(OO_MPI_IO_Base<ItemType>::getID(), 
                           OO_MPI_IO_Base<ItemType>::getNumPEs(),
                           OO_MPI_IO_Base<ItemType>::getNumItemsInFile(),
                           start, stop);

   OO_MPI_IO_Base<ItemType>::setFirstItemOffset(start);
   OO_MPI_IO_Base<ItemType>::setFirstByteOffset(start * itemSize);
   MPI_Status status;
   unsigned long itemsWritten = 0;
   unsigned long itemsToWrite = OO_MPI_IO_Base<ItemType>::getChunkSize();
   int writeResult = 0;
   while (itemsToWrite > INT_MAX) {
     writeResult = MPI_File_write_at(OO_MPI_IO_Base<ItemType>::getFileHandle(), 
                   OO_MPI_IO_Base<ItemType>::getFirstByteOffset()+itemsWritten, 
                    v.data()+itemsWritten,
                    INT_MAX, 
                    OO_MPI_IO_Base<ItemType>::getMPIType(), 
                    &status);
     checkResult(OO_MPI_IO_Base<ItemType>::getID(), writeResult);
     itemsWritten += INT_MAX;
     itemsToWrite -= INT_MAX;
   }

   writeResult = MPI_File_write_at(OO_MPI_IO_Base<ItemType>::getFileHandle(), 
                   OO_MPI_IO_Base<ItemType>::getFirstByteOffset()+itemsWritten, 
                   v.data()+itemsWritten,
                   itemsToWrite, 
                   OO_MPI_IO_Base<ItemType>::getMPIType(), 
                   &status);
   checkResult(OO_MPI_IO_Base<ItemType>::getID(), writeResult);
}

#endif

