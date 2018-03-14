#include "file.h"
using namespace Repository;
//----< push back block byte >---------------------------------------------
Block::Block(Byte* beg, Byte* end) : bytes_(beg, end) {}
void Block::push_back(Byte b)
{
	bytes_.push_back(b);
}
//----< non-const indexer >------------------------------------------------

Byte& Block::operator[](size_t i)
{
	if (i<0 || bytes_.size() <= i)
		throw std::runtime_error("index out of range in Block");
	return bytes_[i];
}
//----< const indexer >----------------------------------------------------

Byte Block::operator[](size_t i) const
{
	if (i<0 || bytes_.size() <= i)
		throw std::runtime_error("index out of range in Block");
	return bytes_[i];
}
//----< equality comparison >----------------------------------------------

bool Block::operator==(const Block& block) const
{
	return bytes_ == block.bytes_;
}
//----< inequality comparison >--------------------------------------------

bool Block::operator!=(const Block& block) const
{
	return bytes_ != block.bytes_;
}
//----< return number of bytes in block >----------------------------------

size_t Block::size() const
{
	return bytes_.size();
}
File::File(const std::string& filespec)
	: name_(filespec), pIStream(nullptr), pOStream(nullptr), dirn_(in), typ_(text), good_(true)
{
}
//----< File destructor closes file stream >-------------------------------

File::~File()
{
	if (pIStream)
	{
		pIStream->close();
		delete pIStream;
		pIStream = nullptr;
		good_ = false;
	}
	if (pOStream)
	{
		pOStream->close();
		delete pOStream;
		pOStream = nullptr;
		good_ = false;
	}
}
//----< open for reading or writing >--------------------------------------

bool File::open(direction dirn, type typ)
{
	dirn_ = dirn;
	typ_ = typ;
	good_ = true;
	if (dirn == in)
	{
		pIStream = new std::ifstream;
		if (typ == binary)
			pIStream->open(name_.c_str(), std::ios::in | std::ios::binary);
		else
			pIStream->open(name_.c_str(), std::ios::in);
		if (!(*pIStream).good())
		{
			good_ = false;
			pIStream = nullptr;
			//throw std::runtime_error("\n  open for input failed in File constructor");
		}
	}
	else
	{
		pOStream = new std::ofstream;
		if (typ == binary)
			pOStream->open(name_.c_str(), std::ios::out | std::ios::binary);
		else
			pOStream->open(name_.c_str(), std::ios::out);
		if (!(*pOStream).good())
		{
			good_ = false;
			pOStream = nullptr;
			//throw std::runtime_error("\n  open for output failed in File constructor");
		}
	}
	return good_;
}
//----< reads one line of a text file >------------------------------------

std::string File::getLine(bool keepNewLines)
{
	if (pIStream == nullptr || !pIStream->good())
		throw std::runtime_error("input stream not open");
	if (typ_ == binary)
		throw std::runtime_error("getting text line from binary file");
	if (dirn_ == out)
		throw std::runtime_error("reading output file");

	std::string store;
	while (true)
	{
		char ch = pIStream->get();
		if (!isGood())
			return store;
		if (ch == '\n')
		{
			if (keepNewLines)
				store += ch;
			return store;
		}
		store += ch;
	}
}
//----< read all lines of text file into one string >----------------------

std::string File::readAll(bool keepNewLines)
{
	std::string store;
	while (true)
	{
		if (!isGood())
			return store;
		store += getLine(keepNewLines);
		std::locale loc;
		if (store.size() > 0 && !std::isspace(store[store.size() - 1], loc))
			store += ' ';
	}
	return store;
}
//----< writes one line of a text to a file >------------------------------

void File::putLine(const std::string& s, bool wantReturn)
{
	if (pOStream == nullptr || !pOStream->good())
		throw std::runtime_error("output stream not open");
	if (typ_ == binary)
		throw std::runtime_error("writing text line to binary file");
	if (dirn_ == in)
		throw std::runtime_error("writing input file");
	for (size_t i = 0; i<s.size(); ++i)
		pOStream->put(s[i]);
	if (wantReturn)
		pOStream->put('\n');
	pOStream->flush();
}
//----< reads a block of bytes from binary file >--------------------------

Block File::getBlock(size_t size)
{
	if (pIStream == nullptr || !pIStream->good())
		throw std::runtime_error("input stream not open");
	if (typ_ != binary)
		throw std::runtime_error("reading binary from text file");
	if (dirn_ == out)
		throw std::runtime_error("reading output file");
	Block blk;
	if (pIStream)
	{
		for (size_t i = 0; i<size; ++i)
		{
			Byte b;
			pIStream->get(b);
			if (pIStream->good())
				blk.push_back(b);
			else
				break;
		}
	}
	return blk;
}
//----< writes a block of bytes to binary file >---------------------------

void File::putBlock(const Block& blk)
{
	if (pOStream == nullptr || !pOStream->good())
		throw std::runtime_error("output stream not open");
	if (typ_ != binary)
		throw std::runtime_error("writing binary to text file");
	if (dirn_ == in)
		throw std::runtime_error("writing input file");
	if (!pOStream->good())
		return;
	for (size_t i = 0; i<blk.size(); ++i)
	{
		pOStream->put(blk[i]);
	}
}
//----< read buffer of bytes from binary file >----------------------------

size_t File::getBuffer(size_t bufLen, File::byte* buffer)
{
	if (pIStream == nullptr || !pIStream->good())
		throw std::runtime_error("input stream not open");
	if (typ_ != binary)
		throw std::runtime_error("reading binary from text file");
	if (dirn_ == out)
		throw std::runtime_error("reading output file");
	size_t count = 0;
	while (pIStream->good())
	{
		buffer[count++] = pIStream->get();
		if (count == bufLen)
			break;
	}
	if (!pIStream->good())  // don't write EOF char
		--count;
	return count;
}
//----< write buffer of bytes to binary file >-------------------------------

void File::putBuffer(size_t bufLen, File::byte* buffer)
{
	if (pOStream == nullptr || !pOStream->good())
		throw std::runtime_error("output stream not open");
	if (typ_ != binary)
		throw std::runtime_error("writing binary to text file");
	if (dirn_ == in)
		throw std::runtime_error("writing input file");
	if (!pOStream->good())
		return;
	size_t count = 0;
	while (pOStream->good())
	{
		pOStream->put(buffer[count++]);
		if (count == bufLen)
			break;
	}
}
//----< tests for error free stream state >--------------------------------

bool File::isGood()
{
	if (!good_)
		return false;
	if (pIStream != nullptr)
		return (good_ = pIStream->good());
	if (pOStream != nullptr)
		return (good_ = pOStream->good());
	return (good_ = false);
}
//----< flushes output stream to its file >--------------------------------

void File::flush()
{
	if (pOStream != nullptr && pOStream->good())
		pOStream->flush();
}
//----< clears error state enabling operations again >---------------------

void File::clear()
{
	if (pIStream != nullptr)
		pIStream->clear();
	if (pOStream != nullptr)
		pOStream->clear();
}
//----< close file handle >------------------------------------------------

void File::close()
{
	File::flush();
	if (pIStream != nullptr)
	{
		pIStream->close();
		pIStream = nullptr;
		good_ = false;
	}
	if (pOStream)
	{
		pOStream->close();
		pOStream = nullptr;
		good_ = false;
	}
}
//----< file exists >--------------------------------------------------

bool File::exists(const std::string& file)
{
	return ::GetFileAttributesA(file.c_str()) != INVALID_FILE_ATTRIBUTES;
}
//----< copy file >----------------------------------------------------

bool File::copy(const std::string& src, const std::string& dst, bool failIfExists)
{
	return ::CopyFileA(src.c_str(), dst.c_str(), failIfExists) != 0;
}
//----< remove file >--------------------------------------------------

bool File::remove(const std::string& file)
{
	return ::DeleteFileA(file.c_str()) != 0;
}
//----< convert string to lower case chars >---------------------------

std::string Path::toLower(const std::string& src)
{
	std::string temp;
	for (size_t i = 0; i<src.length(); ++i)
		temp += tolower(src[i]);
	return temp;
}
//----< convert string to upper case chars >---------------------------


std::string Path::toUpper(const std::string& src)
{
	std::string temp;
	for (size_t i = 0; i<src.length(); ++i)
		temp += toupper(src[i]);
	return temp;
}
//----< get path from fileSpec >---------------------------------------

std::string Path::getName(const std::string &fileSpec, bool withExt)
{
	size_t pos = fileSpec.find_last_of("/");
	if (pos >= fileSpec.length())
	{
		pos = fileSpec.find_last_of("\\");
		if (pos >= fileSpec.length())
		{
			// no path prepended
			if (withExt)
				return fileSpec;
			else
			{
				// remove ext
				size_t pos = fileSpec.find(".");
				if (pos > fileSpec.size())
					return fileSpec;
				return fileSpec.substr(0, pos - 1);
			}
		}
	}
	if (withExt)
		return fileSpec.substr(pos + 1, fileSpec.length() - pos);
	else
	{
		// remove ext
		size_t pos2 = fileSpec.find(".", pos);
		if (pos2 > fileSpec.size())
			// no ext
			return fileSpec.substr(pos + 1);
		return fileSpec.substr(pos + 1, pos2 - pos - 1);
	}
}
//----< get extension from fileSpec >----------------------------------

std::string Path::getExt(const std::string& fileSpec)
{
	size_t pos1 = fileSpec.find_last_of('/');
	size_t pos2 = fileSpec.find_last_of('\\');
	size_t pos = fileSpec.find_last_of('.');
	// handle ../ or ..\\ with no extension
	if (pos1 < fileSpec.length() || pos2 < fileSpec.length())
	{
		if (pos < min(pos1, pos2))
			return std::string("");
	}
	// only . is extension delimiter
	if (0 <= pos && pos < fileSpec.length())
		return toLower(fileSpec.substr(pos + 1, fileSpec.length() - pos));
	return std::string("");
}
//----< get path from fileSpec >---------------------------------------

std::string Path::getPath(const std::string &fileSpec)
{
	size_t pos = fileSpec.find_last_of("/");
	if (pos >= fileSpec.length())
		pos = fileSpec.find_last_of("\\");
	if (pos >= fileSpec.length())
		return ".";
	if (fileSpec.find(".", pos + 1))
		return fileSpec.substr(0, pos + 1);
	return fileSpec;
}
//----< get absoluth path from fileSpec >------------------------------

std::string Path::getFullFileSpec(const std::string &fileSpec)
{
	const size_t BufSize = 256;
	char buffer[BufSize];
	char filebuffer[BufSize];  // don't use but GetFullPathName will
	char* name = filebuffer;
	::GetFullPathNameA(fileSpec.c_str(), BufSize, buffer, &name);
	return std::string(buffer);
}
//----< create file spec from path and name >--------------------------

std::string Path::fileSpec(const std::string &path, const std::string &name)
{
	std::string fs;
	size_t len = path.size();
	if (path[len - 1] == '/' || path[len - 1] == '\\')
		fs = path + name;
	else
	{
		if (path.find("/") < path.size())
			fs = path + "/" + name;
		else if (path.find("\\") < path.size())
			fs = path + "\\" + name;
		else
			fs = path + "/" + name;
	}
	return fs;
}
//----< return name of the current directory >-----------------------------

std::string Directory::getCurrentDirectory()
{
	char buffer[MAX_PATH];
	::GetCurrentDirectoryA(MAX_PATH, buffer);
	return std::string(buffer);
}
//----< change the current directory to path >-----------------------------

bool Directory::setCurrentDirectory(const std::string& path)
{
	return ::SetCurrentDirectoryA(path.c_str()) != 0;
}
//----< get names of all the files matching pattern (path:name) >----------

std::vector<std::string> Directory::getFiles(const std::string& path, const std::string& pattern)
{
	std::vector<std::string> files;
	FileSystemSearch fss;
	std::string file = fss.firstFile(path, pattern);
	if (file.size() == 0)
		return files;
	files.push_back(file);
	while (true)
	{
		file = fss.nextFile();
		if (file.size() == 0)
			return files;
		files.push_back(file);
	}
	return files;
}
//----< get names of all directories matching pattern (path:name) >--------

std::vector<std::string> Directory::getDirectories(const std::string& path, const std::string& pattern)
{
	std::vector<std::string> dirs;
	FileSystemSearch fss;
	std::string dir = fss.firstDirectory(path, pattern);
	if (dir.size() == 0)
		return dirs;
	dirs.push_back(dir);
	while (true)
	{
		dir = fss.nextDirectory();
		if (dir.size() == 0)
			return dirs;
		dirs.push_back(dir);
	}
	return dirs;
}
//----< create directory >-------------------------------------------------

bool Directory::create(const std::string& path)
{
	return ::CreateDirectoryA(path.c_str(), NULL) == 0;
}
//----< does directory exist? >--------------------------------------------

bool Directory::exists(const std::string& path)
{
	DWORD dwAttrib = GetFileAttributesA(path.c_str());

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}
//----< remove directory >-------------------------------------------------

bool Directory::remove(const std::string& path)
{
	return ::RemoveDirectoryA(path.c_str()) == 0;
}
std::string FileSystemSearch::firstFile(const std::string& path, const std::string& pattern)
{
	hFindFile = ::FindFirstFileA(Path::fileSpec(path, pattern).c_str(), pFindFileData);
	if (hFindFile != INVALID_HANDLE_VALUE)
	{
		if (!(pFindFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			return pFindFileData->cFileName;
		else
			while (::FindNextFileA(hFindFile, pFindFileData))
				if (!(pFindFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
					return pFindFileData->cFileName;
	}
	return "";
}
//----< find next file >---------------------------------------------------

std::string FileSystemSearch::nextFile()
{
	while (::FindNextFileA(hFindFile, pFindFileData))
		if (!(pFindFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			return pFindFileData->cFileName;
	return "";
}
//----< find first file >--------------------------------------------------

std::string FileSystemSearch::firstDirectory(const std::string& path, const std::string& pattern)
{
	hFindFile = ::FindFirstFileA(Path::fileSpec(path, pattern).c_str(), pFindFileData);
	if (hFindFile != INVALID_HANDLE_VALUE)
	{
		if (pFindFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			return pFindFileData->cFileName;
		else
			while (::FindNextFileA(hFindFile, pFindFileData))
				if (pFindFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					return pFindFileData->cFileName;
	}
	return "";
}

//-------Test File manager functions
#ifdef TEST_FILEMGR
void title(const std::string& title, char ch = '=')
{
	std::cout << "\n  " << title;
	std::cout << "\n " << std::string(title.size() + 2, ch);
}
int main() {
	title("Demonstrate File class operations", '=');
	std::cout << "\n";

	// copy binary file from one directory to another

	File me("../CoreRepository/CoreRepository.h");
	me.open(File::in, File::binary);
	if (!Directory::exists("c:/temp"))Directory::create("c:/temp");
	std::cout << "\n  copying " << me.name().c_str() << " to c:/temp";
	if (!me.isGood())
	{
		std::cout << "\n  can't open executable\n";
		std::cout << "\n  looking for:\n  ";
		std::cout << Path::getFullFileSpec(me.name()) << "\n";
	}
	else
	{
		File you("c:/temp/CoreRepository.h.v1");
		you.open(File::out, File::binary);
		if (you.isGood())
		{
			while (me.isGood())
			{
				static size_t count = 0;
				Block b = me.getBlock(1024);
				you.putBlock(b);
				if (++count < 10)
				{
					std::cout << "\n    reading block of " << b.size() << " bytes";
					std::cout << "\n    writing block of " << b.size() << " bytes";
				}
				if (b.size() < 1024)
				{
					std::cout << "\n\n    omitted " << count - 10 << " blocks from display\n\n";
					std::cout << "\n    reading block of " << b.size() << " bytes";
					std::cout << "\n    writing block of " << b.size() << " bytes";
				}
			}
			std::cout << "\n";
		}
	}

}

#endif // 
