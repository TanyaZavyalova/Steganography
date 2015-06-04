#include <vector>
#include <fstream>
#include <iterator>
#include <string>
#include <bitset>


using namespace std;

vector<unsigned char> readFile(ifstream &inputFile)
{
	vector<unsigned char> buffer((std::istreambuf_iterator<char>(inputFile)), (std::istreambuf_iterator<char>()));
    
	return buffer;
}

int main(int argc, const char * argv[])
{
	string wav = "WAVE";
	string tmp = "";
	tmp.reserve(4);
	if (argc < 3)
	{
		cout << "Error: Too few arguments." << endl;
		return 0;
	}
    
	ifstream wavFile(argv[1], ios::binary);
	if (!wavFile)
	{
		cout << "Error: Unable to open file " << argv[1] << endl;
		return 0;
	}
    
	ifstream txtFile(argv[2], ios::binary);
	if (!txtFile)
	{
		cout << "Error: Unable to open file " << argv[2] << endl;
		return 0;
	}
    
	vector<unsigned char> wavBytes = readFile(wavFile);
	vector<unsigned char> txtBytes = readFile(txtFile);

	for (int i = 8; i < 12; i++)
	{
		tmp += wavBytes[i];
	}
	if (tmp != wav)
	{
		cout << "It's not wav file! ;(";
		return 0;
	}

	if (wavBytes.size() - 44 < txtBytes.size() * 8)
	{
		cout << "Error: Wav file capasity is too low." << endl;
		return 0;
	}
    
	size_t j = 44;
	for (size_t i = 0; i < txtBytes.size(); i++)
	{
		bitset<8> txtByte(txtBytes[i]);
        
		for (long long k = txtByte.size() - 1; k > -1; k--)
		{
			bitset<8> wavByte(wavBytes[j]);
			wavByte[0] = txtByte[k];
            
			unsigned long tmp = wavByte.to_ulong();
			wavBytes[j] = static_cast<unsigned char>(tmp);
            
			++j;
		}
        
	}
	
	
	ofstream outputFile("steganographyOutput.wav", ios::binary);
    
	for (size_t i = 0; i < wavBytes.size(); i++)
	{
		outputFile << static_cast<unsigned char>(wavBytes[i]);
	}
    
    
	outputFile.close();
	return 0;
    
}

