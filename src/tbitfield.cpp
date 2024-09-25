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

TBitField::TBitField(int len) :BitLen(len)
{
    if (len <= 0) throw - 1;
    MemLen = len / (sizeof(TELEM) * 8) + 1;
    pMem = new TELEM[MemLen];

    for (int i = 0; i < len; i++)pMem[i] = 0;
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    MemLen = bf.MemLen;
    BitLen = bf.BitLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {// непон MEmlen
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

void TBitField::checkLen(int n)
{
    if (n < 0 || n >= BitLen)
    {
        throw - 1;
    }
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
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
    return FAKE_INT;
}

void TBitField::SetBit(const int n) // установить бит
{
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    checkLen(n);
    int i = GetMemIndex(n);
    TELEM t = GetMemMask(n);
    pMem[i] &= t;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    checkLen(n);
    int i = GetMemIndex(n);
    TELEM t = GetMemMask(n);
    return pMem[i] &= t;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (this == &bf) return *this;

    if (MemLen != bf.MemLen) {
        MemLen = bf.MemLen;
        delete[] pMem;
        pMem = new TELEM[MemLen];
    }

    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }

    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (this->BitLen != bf.BitLen)return 0;
    for (int i = 0; i < BitLen; i++) {
        if (this->GetBit(i) != bf.GetBit(i))return 0;
    }
    return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    if (this->BitLen == bf.BitLen)return 0;
    for (int i = 0; i < BitLen; i++) {
        if (this->GetBit(i) == bf.GetBit(i))return 0;
    }
    return 1;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    return FAKE_BITFIELD;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    int tmp = bf.BitLen;
    int ltmp = this->BitLen;
    TBitField res(tmp);
    int i = 0;
    for (i = 0; i < ltmp; i++) {
        res.pMem[i] = this->pMem[i] & bf.pMem[i];
    }
    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField result(BitLen);
    for (int i = 0; i < MemLen; i++) {
        result.pMem[i] = ~pMem[i];
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
