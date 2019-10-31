#include <bits/stdc++.h> //reverse
#include <fstream>
#include <iostream>
#include <vector>
#include <string>


class Read {
    public:
        int pos;
        std::string seq;

        Read(): pos(-1), seq("") { }
        Read(int _pos, std::string _seq): pos(_pos), seq(_seq) { }
        Read & operator=(const Read & other) {
            pos = other.pos;
            seq = other.seq;
            return *this;
        }
};

Read empty_read = Read();

class Suffix {
    public:
        bool is_in_ref;
        int pos_in_seq;
        std::string & ref;
        Read * read;

        Suffix(bool _in_ref,
                int _pos_in_seq,
                std::string & _ref): 
            is_in_ref(_in_ref),
            pos_in_seq(_pos_in_seq),
            ref(_ref),
            read(&empty_read){ }

        Suffix(bool _in_ref,
                int _pos_in_seq,
                std::string & _ref,
                Read & _read): 
            is_in_ref(_in_ref),
            pos_in_seq(_pos_in_seq),
            ref(_ref),
            read(&_read) { }

        Suffix & operator=(const Suffix & other) {
        is_in_ref = other.is_in_ref;
        pos_in_seq = other.pos_in_seq;
        ref = other.ref;
        read = other.read;
        return *this;
        }

        char get_char(int i) const {
            if (is_in_ref) {
                if (i+pos_in_seq < ref.size()) 
                    return ref[i+pos_in_seq];
                else 
                    return '$';
            }
            else {
                if (i+pos_in_seq < read->seq.size()) 
                    return read->seq[i+pos_in_seq];
                else {
                    int i_ref = read->pos + i + pos_in_seq - read->seq.size();
                    if (i_ref < ref.size())
                        return ref[i_ref];
                    else
                        return '$';
                }
            }
        }

        bool operator<(const Suffix & other) const {
            int i = 0;
            while (true) {
                char c = get_char(i);
                char other_c = other.get_char(i);
                if (c == '$' && other_c == '$')
                    return false;
                if (c == '$')
                    return true;
                if (other_c == '$')
                    return false;
                if (c != other_c)
                    return c < other_c;
                i++;
            }
        }

};

std::ostream &operator<<(std::ostream &os, Read const &r) {
    return os << r.pos << " " << r.seq;
}

std::ostream &operator<<(std::ostream &os, Suffix const &s) {
    if (s.is_in_ref){
        return os << "ref " << s.pos_in_seq;
    }
    else {
        return os << "read " << s.pos_in_seq << " " << *s.read;
    }
}

int main(int argc,char* argv[]) {
    if (argc==1) {
        std::cout << "You must give the input file as an argument after the program name." << std::endl;
        return 1;
    }
    else {
        std::string ref;
        std::vector<Read> reads;
        std::ifstream inFile;
        inFile.open(argv[1]);
        if (!inFile) {
            std::cout << "Unable to open file";
            exit(1); // terminate with error
        }

        //parse custom format
        inFile >> ref;
        std::reverse(ref.begin(),ref.end());
        int pos;
        std::string seq;
        /*while (inFile >> pos >> seq){
            reverse(seq.begin(),seq.end());
            reads.push_back(Read(ref.size()-pos,seq));
        }*/
        std::cout << "Finished parsing input" << std::endl;

        //creation of the sa
        std::vector<Suffix> sa;
        std::vector<std::vector<int>> start_of_read(ref.size()+1);
        for(int i = 0; i < ref.size(); i++) {
            sa.push_back(Suffix(true,i,ref));
        }
        /*for(int r = 0; r < reads.size(); r++) {
            start_of_read[reads[r].pos].push_back(r);
            for(int i= 0; i < reads[r].seq.size(); i++){
                sa.push_back(Suffix(false,i,ref,reads[r]));
            }
        }*/
        std::cout << "Finished adding to sa" << std::endl;

        std::sort(sa.begin(),sa.end());
        std::cout << "Finished ordering sa" << std::endl;

        //creation of the bwt
        std::vector<char> bwt;
        bwt.push_back(ref.back());
        for(int i = 0; i < start_of_read[ref.size()].size(); i++)
            bwt.push_back(reads[i].seq.back());
        for(int i = 0; i < sa.size(); i++) {
            if (sa[i].pos_in_seq > 0){
                bwt.push_back(sa[i].get_char(-1));
                if (sa[i].is_in_ref){
                for(int j = 0; j < start_of_read[sa[i].pos_in_seq].size(); j++){
                    bwt.push_back(reads[start_of_read[sa[i].pos_in_seq][j]].seq.back());
                }
                }
            }
        }
        std::cout << "Finished building bwt" << std::endl;

        //output the result
        std::cout <<"BWT:" << std::endl;
        /*
        for(int i = 0; i < bwt.size(); i++)
            std::cout << bwt[i]; 
        */
        std::cout << std::endl;
        char last = bwt[0];
        int nb_last = 1;
        int nb_runs = 1;
        for(int i = 1; i < bwt.size(); i++) {
            if (bwt[i] == last)
                nb_last++;
            else {
                std::cout << last << nb_last;
                last = bwt[i];
                nb_last = 1;
                nb_runs++;
            }
        }
        std::cout << std::endl;
        std::cout << "Number of runs: " << nb_runs << std::endl;
        std::cout << "average size of run: " << sa.size()/nb_runs << std::endl;

    }
    return 0;
}
