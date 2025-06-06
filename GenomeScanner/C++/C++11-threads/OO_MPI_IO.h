/* OO_MPI_IO.h declares C++ templates that use  MPI_IO:
 *  - ParallelReader to read data from a binary file in parallel.
 *  - ParallelWriter to write data to a binary file in parallel.
 *
 * The template allows you to pass a type-parameter indicating
 *   the type of the data in the file.
 *
 * @author: Joel C. Adams, for CS 374 at Calvin University, Fall 2023.
 *
 */

#ifndef OO_MPI_IO
#define OO_MPI_IO

#include <mpi.h>                     // C MPI
#include <string>                    // C++ string 
#include <cmath>                     // ceil()
#include <vector>                    // C++ vector

/********************************************************************
 * OO_MPI_IO_Base is a base class for C++ templates that use
 *  MPI_IO to read/write binary data from/to files in parallel.
 *
 * Its subclasses are ParallelReader and ParallelWriter.
 ********************************************************************/

template<class ItemType> 
class OO_MPI_IO_Base {
public:
  OO_MPI_IO_Base(const std::string& fileName, int openMode,
                   MPI_Datatype mpiType,
                   int rank, int numProcs);
  virtual ~OO_MPI_IO_Base()        { MPI_File_close(&myFileHandle); }
 
  int getRank() const              { return myRank; }
  int getNumProcs() const          { return myNumProcs; }
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
  int          myRank;                // MPI process ID
  int          myNumProcs;            // number of MPI processes
  int          myItemSize;            // size of 1 Item
  std::string  myFileName;            // file being opened
  MPI_Datatype myMPIType;             // the MPI equiv of ItemType
  MPI_File     myFileHandle;          // MPI handle for file 

  // these attributes are unknown until read or write is called
  long         myNumItemsInFile;      // total Items to be read
  MPI_Offset   myFileSize;            // size of file in bytes
  long         myChunkSize;           // size of my chunk to read
  long         myFirstItemOffset;     // offset of my chunk (Item #)
  long         myFirstByteOffset;     // offset of my chunk (byte #)
};

/* Utility to check the return-values of MPI-IO function calls
 * @param: id, an int
 * @param: result, an int
 * Precondition: id is the rank of the MPI process invoking this function 
 *           &&  result is the return-value from the last MPI-IO call.
 * Postcondition: If result indicates there was an error:
 *                 the string associated with result has been printed to stderr
 *                 && the program has been terminated abnormally.
 */
void checkResult(int id, int result) {
  if (result != MPI_SUCCESS) {
    char errorString[1024] = {'\0'};
    int  errorStringLength = -1;
    int  errorClass = -1;

    MPI_Error_class(result, &errorClass);
    MPI_Error_string(errorClass, errorString, &errorStringLength);
    fprintf(stderr, "Process %d: %s\n", id, errorString);

    MPI_Abort(MPI_COMM_WORLD, result);
  }
}


/* OO_MPI_IO_BASE constructor
 * @param: fileName, a string
 * @param: openMode, an int
 * @param: mpiType, an MPI_Datatype value.
 * @param: rank, an int
 * @param: numProcs, an int
 * Precondition: fileName is the name of a file containing
 *                binary-format values of type ItemType
 *           &&  openMode is a valid MPI file-opening mode
 *           &&  mpiType is the MPI equivalent of ItemType
 *           &&  rank is the MPI rank of this process
 *           &&  numProcs is the number of MPI processes.
 * Postcondition: the file has been opened for parallel IO
                   as specified by openMode
 *           &&  each instance variable have been initialized
 *                as appropriate for this process using rank,
 *                numProcs, etc.
 * Note: It would be cleaner to pass mpiType as a template parameter
 *         but doing so produces errors (at least for OpenMPI and clang),
 *         so this is a hack-ey workaround.
 */
template <class ItemType>
OO_MPI_IO_Base<ItemType>::
OO_MPI_IO_Base(const std::string& fileName, int openMode, 
                  MPI_Datatype mpiType,
                  int rank, int numProcs) {
   myFileName = fileName;
   myMPIType = mpiType;
   myRank = rank;
   myNumProcs = numProcs;
   myItemSize = sizeof(ItemType);
   // set other attributes to defaults for now
   myNumItemsInFile = 0;
   myFileSize = 0;
   myChunkSize = 0;
   myFirstItemOffset = 0;
   myFirstByteOffset = 0;

   int openResult = MPI_File_open( MPI_COMM_WORLD,    // communicator
                                    fileName.c_str(), // name of file
                                    openMode,         // mode parameter
                                    MPI_INFO_NULL,    // no info
                                    &myFileHandle );  // MPI handle
   checkResult(rank, openResult);
}


/* Calculate the start and stop values for this MPI process's 
 *  contiguous chunk of a set of loop-iterations, 0..REPS-1,
 *  so that each process chunk-sizes are equal (or nearly so).
 *
 * @param: id, an int containing this process's MPI rank
 * @param: numProcs, an int containing the number of processes
 * @param: REPS, a const unsigned containing the for loop's iteration total
 * Precondition: id == this process's MPI rank
 *            && numProcs == the number of MPI processes
 *            && REPS == the total number of 0-based loop iterations needed
 *            && numProcs <= REPS 
 *            && REPS < 2^32
 * @param: start, a long reference through which the 
 *          starting value of this process's chunk should be returned
 * @param: stop, a long reference through which the
 *          stopping value of this process's chunk should be returned
 * Postcondition: start == this process's first iteration value 
 *             && stop == this process's last iteration value + 1.
 */
void getChunkStartStopValues(int id, int numProcs, const unsigned REPS,
                              long& start, long& stop)
{
   // check precondition before proceeding
   if ((unsigned) numProcs > REPS) {
      if (id == 0) {
         printf("\n*** Number of MPI processes (%u) exceeds REPS (%u)\n",
                 numProcs, REPS);
         printf("*** Please run with -np less than or equal to %u\n\n", REPS);
      }
      MPI_Finalize();
      exit(1);
   }

   // compute the chunk size that works in many cases
   unsigned chunkSize1 = (long)ceil(((double)REPS) / numProcs);
   unsigned begin = id * chunkSize1;
   unsigned end = begin + chunkSize1;
   // see if there are any leftover iterations
   unsigned remainder = REPS % numProcs;
   // If remainder == 0, chunkSize1 = chunk-size for all processes;
   // If remainder != 0, chunkSize1 = chunk-size for p_0..p_remainder-1
   //   but for processes p_remainder..p_numProcs-1
   //   recompute begin and end using a smaller-by-1 chunk size, chunkSize2.
   if (remainder > 0 && (unsigned) id >= remainder) {
     unsigned chunkSize2 = chunkSize1 - 1;
     unsigned remainderBase = remainder * chunkSize1;
     unsigned processOffset = (id-remainder) * chunkSize2;
     begin = remainderBase + processOffset;
     end = begin + chunkSize2;
   } 
   // pass back this proc's begin and end values via start and stop
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
                   int rank, int numProcs);
  std::vector<ItemType> readChunk();
  std::vector<ItemType> readChunk(unsigned numExtras);
};

/* ParallelReader constructor
 * @param: fileName, a string
 * @param: mpiType, an MPI_Datatype value.
 * Precondition: mpiType is the MPI_Datatype that corresonds to ItemType
 *               (e.g., MPI_DOUBLE for double).
 * @param: rank, an int
 * @param: numProcs, an int
 * Precondition: fileName is the name of a file containing
 *                binary-format values of type ItemType
 *           &&  rank is the MPI rank of this process
 *           &&  numProcs is the number of MPI processes.
 * Postcondition: the file has been opened for parallel input
 *           &&  each instance variable have been initialized
 *                as appropriate for this process using rank,
 *                numProcs, and size info from the file.
 */
template <class ItemType>
ParallelReader<ItemType>::
ParallelReader(const std::string& fileName, MPI_Datatype mpiType,
                 int rank, int numProcs) 
: OO_MPI_IO_Base<ItemType>(fileName, MPI_MODE_RDONLY, mpiType, rank, numProcs)
{ }

/* method to read a chunk from the file
 * Return: a vector containing the values of this process' chunk.
 * Note: vector was chosen as the return-type b/c it uses
 *         contiguous memory and it provides a move-constructor.
 */
template <class ItemType>
std::vector<ItemType> 
ParallelReader<ItemType>::readChunk() {
   // Note: We could compute the following attributes in the constructor, 
   //  but do them here for symmetry with ParallelWriter
   MPI_Offset fileSize;
   MPI_File_get_size(OO_MPI_IO_Base<ItemType>::getFileHandle(), &fileSize);
   OO_MPI_IO_Base<ItemType>::setFileSize(fileSize);
   OO_MPI_IO_Base<ItemType>::setNumItemsInFile( fileSize / OO_MPI_IO_Base<ItemType>::getItemSize() );

   long start = 0, stop = 0;
   getChunkStartStopValues(OO_MPI_IO_Base<ItemType>::getRank(), 
                           OO_MPI_IO_Base<ItemType>::getNumProcs(),
                           OO_MPI_IO_Base<ItemType>::getNumItemsInFile(),
                           start, stop);
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
      checkResult(OO_MPI_IO_Base<ItemType>::getRank(), readResult);
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
   checkResult(OO_MPI_IO_Base<ItemType>::getRank(), readResult);

   return v;
}

/* method to read a chunk from the file plus a few items from
 *  the next process's chunk (for searching problems).
 * @Parameter: numExtras, an unsigned.
 * Return: a vector containing the values of this process's chunk
 *          plus numExtras values of the next process's chunk.
 * Note: vector was chosen as the return-type b/c it uses
 *         contiguous memory and it provides a move-constructor.
 */
template <class ItemType>
std::vector<ItemType>
ParallelReader<ItemType>::readChunk(unsigned numExtras) {
   // Note: We could compute the following attributes in the constructor,
   //  but do them here for symmetry with ParallelWriter
   MPI_Offset fileSize;
   MPI_File_get_size(OO_MPI_IO_Base<ItemType>::getFileHandle(), &fileSize);
   OO_MPI_IO_Base<ItemType>::setFileSize(fileSize);
   OO_MPI_IO_Base<ItemType>::setNumItemsInFile( fileSize / OO_MPI_IO_Base<ItemType>::getItemSize() );

   int id = OO_MPI_IO_Base<ItemType>::getRank();
   int numProcs = OO_MPI_IO_Base<ItemType>::getNumProcs();
   long numItemsInFile = OO_MPI_IO_Base<ItemType>::getNumItemsInFile();
   long start = 0, stop = 0;
   getChunkStartStopValues(id, numProcs, numItemsInFile, start, stop);
   if (id < numProcs-1) {
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
      checkResult(OO_MPI_IO_Base<ItemType>::getRank(), readResult);
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
   checkResult(OO_MPI_IO_Base<ItemType>::getRank(), readResult);

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
                 int rank, int numProcs);
  void writeChunk(const std::vector<ItemType>& v);
 
private:
};


/* ParallelWriter constructor
 * @param: fileName, a string
 * @param: rank, an int
 * @param: numProcs, an int
 * Precondition: fileName is the name of an output file 
 *                to which binary-format values 
 *                of type ItemType are to be written.
 *             && mpiType is the MPI equivalent of ItemType
 *                 (e.g., MPI_DOUBLE for double).
 *             &&  rank is the MPI rank of this process
 *             &&  numProcs is the number of MPI processes.
 * Postcondition: the file has been opened for parallel output
 *             &&  each instance variable have been initialized
 *                  as appropriate for this process using rank,
 *                  numProcs, and size info from the file.
 */
template <class ItemType>
ParallelWriter<ItemType>::
ParallelWriter(const std::string& fileName, MPI_Datatype mpiType,
                 int rank, int numProcs) 
: OO_MPI_IO_Base<ItemType>(fileName,
                            MPI_MODE_WRONLY | MPI_MODE_CREATE,  
                            mpiType, rank, numProcs)
{}  // no instance variables, so no local initializations


/* method to write this process's chunk to the file
 * @param: v, a vector of Items.
 * Note: It would be cleaner to pass mpiType as a template parameter
 *         but doing so produces errors (at least for OpenMPI and clang),
 *         so this is a hack-ey workaround.
 *       Could instead pass it as a parameter to the constructor...
 * Postcondition: v's values have been written to the file
 *         at the appropriate offsets for this MPI process.
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
   getChunkStartStopValues(OO_MPI_IO_Base<ItemType>::getRank(), 
                           OO_MPI_IO_Base<ItemType>::getNumProcs(),
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
     checkResult(OO_MPI_IO_Base<ItemType>::getRank(), writeResult);
     itemsWritten += INT_MAX;
     itemsToWrite -= INT_MAX;
   }

   writeResult = MPI_File_write_at(OO_MPI_IO_Base<ItemType>::getFileHandle(), 
                   OO_MPI_IO_Base<ItemType>::getFirstByteOffset()+itemsWritten, 
                   v.data()+itemsWritten,
                   itemsToWrite, 
                   OO_MPI_IO_Base<ItemType>::getMPIType(), 
                   &status);
   checkResult(OO_MPI_IO_Base<ItemType>::getRank(), writeResult);
}

#endif

