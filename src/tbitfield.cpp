// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len) 
{
    if (len < 0)
        throw - 1;
    BitLen = len;
    MemLen = (BitLen + (sizeof(TELEM) * 8 - 1)) / (sizeof(TELEM) * 8);
    pMem = new TELEM[MemLen];
    std::memset(pMem, 0, MemLen * sizeof(TELEM));
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    std::memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));
}

TBitField::~TBitField()
{
    delete[] pMem;
}
void TBitField::checkLen(int n) const 
{
    if (n < 0 || n >= BitLen)
    {
        throw - 1;
    }
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    checkLen(n);
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    checkLen(n);
    return  1 << n % (sizeof(TELEM) * 8);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    checkLen(n);
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    checkLen(n);
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    checkLen(n);
    return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (this != &bf) {
        delete[] pMem;
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++)
            pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (BitLen != bf.BitLen) return 0;
    for (int i = 0; i < MemLen - 1; i++)
        if (pMem[i] != bf.pMem[i]) return 0;
    for (int i = ((MemLen - 1) * sizeof(TELEM) * 8); i < BitLen; i++)
        if (GetBit(i) != bf.GetBit(i)) return 0;
    return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    int len = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    len = std::max(BitLen, bf.BitLen);
    TBitField res(len);
    for (int i = 0; i < MemLen; i++)
        res.pMem[i] = (pMem[i] | bf.pMem[i]);
    return res;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    int len = bf.BitLen;
    if (BitLen > bf.BitLen)
        len = BitLen;
    TBitField res(len);
    for (int i = 0; i < res.MemLen; i++)
        res.pMem[i] = pMem[i] & bf.pMem[i];
    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField result(BitLen);
    for (int i = 0; i < MemLen; i++) {
        result.pMem[i] = ~pMem[i];
    }
    int lem = (1 << BitLen % (sizeof(TELEM) * 8)) - 1;
    if (BitLen % (sizeof(TELEM) * 8) != 0) {
        result.pMem[MemLen - 1] &= lem;
    }
    return result;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    int bit;
    for (int i = 0; i < bf.BitLen; i++) {
        istr >> bit;
        if (bit) {
            bf.SetBit(i);
        }
        else {
            bf.ClrBit(i);
        }
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++) {
        if (bf.GetBit(i) == 0) {
            ostr << 0;
        }
        else {
            ostr << 1;
        }
    }
    return ostr;
}
