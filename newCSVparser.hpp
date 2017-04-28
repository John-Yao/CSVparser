#ifndef     _CSVPARSER_HPP_
# define    _CSVPARSER_HPP_

# include <stdexcept>
# include <string>
# include <vector>
# include <list>
# include <sstream>
#include<iostream>
#include<fstream>
namespace csv
{
    class Error : public std::runtime_error
    {

      public:
        Error(const std::string &msg):
          std::runtime_error(std::string("CSVparser : ").append(msg))
        {
        }
    };

    class Row
    {
    	public:
    	    Row(const std::vector<std::string> &);
    	    ~Row(void);

    	public:
            unsigned int size(void) const;
            void push(const std::string &);
            bool set(const std::string &, const std::string &); 

    	private:
    		const std::vector<std::string> _header;
    		std::vector<std::string> _values;

        public:

            template<typename T>
            const T getValue(unsigned int pos) const
            {
                if (pos < _values.size())
                {
                    T res;
                    std::stringstream ss;
                    ss << _values[pos];
                    ss >> res;
                    return res;
                }
                throw Error("can't return this value (doesn't exist)");
            }
            const std::string operator[](unsigned int) const;
            const std::string operator[](const std::string &valueName) const;
            friend std::ostream& operator<<(std::ostream& os, const Row &row);
            friend std::ofstream& operator<<(std::ofstream& os, const Row &row);
			//extend by yaoyuehan on 20170428 
			//add << for fstream
			friend std::fstream& operator<<(std::fstream& os, const Row &row);
    };

//extend by yaoyuehan on 20170428 
//remove DataType for DataType::ePure is unstable
	enum OPENTYPE
	{
		NEW = 0,
		EXTEND 
	};
    class Writer
    {
    public:
		Writer::Writer(const std::string &fname, std::string header, char sep = ',', const OPENTYPE &type = NEW);
        ~Writer(void);

    public:
        Row &getRow(unsigned int row) const;
        unsigned int rowCount(void) const;
        unsigned int columnCount(void) const;
        std::vector<std::string> getHeader(void) const;
        const std::string getHeaderElement(unsigned int pos) const;
        const std::string &getFileName(void) const;

    public:
        bool deleteRow(unsigned int row);
		bool addRow(const std::vector<std::string> &r);
        bool addRow(unsigned int pos, const std::vector<std::string> &);

    protected:
        void parseHeader(void);
        void parseContent(void);
        void sync(void) ;
    private:
        std::string _fname;
        std::fstream _fileStream;
        const char _sep;
		const OPENTYPE _openType;
        std::vector<std::string> _originalFile; //raw file orgnaized by row
        std::vector<std::string> _header; //
        std::vector<Row *> _content; //raw file orgnaized by row except header

    public:
        Row &operator[](unsigned int row) const;
    };
}

#endif /*!_CSVPARSER_HPP_*/
