#include "SocketAddress.h"



SocketAddress::SocketAddress()
{
	sa_in.sin_family = AF_INET;
	sa_in.sin_addr.S_un.S_addr = 0;
	sa_in.sin_port = 0;
	for (size_t i = 0; i < 8; i++)
	{
		sa_in.sin_zero[i] = 0;
	}
}

SocketAddress::SocketAddress(uint8_t num1, uint8_t num2, uint8_t num3, uint8_t num4, uint16_t puerto)
{
	sa_in.sin_family = AF_INET;
	sa_in.sin_addr.S_un.S_un_b.s_b1 = num1;
	sa_in.sin_addr.S_un.S_un_b.s_b2 = num2;
	sa_in.sin_addr.S_un.S_un_b.s_b3 = num3;
	sa_in.sin_addr.S_un.S_un_b.s_b4 = num4;
	sa_in.sin_port = puerto;
	for (size_t i = 0; i < 8; i++)
	{
		sa_in.sin_zero[i] = 0;
	}
}

SocketAddress::SocketAddress(const SocketAddress & sa)
{
	memcpy(&sa_in, &sa.sa_in, sizeof(sockaddr_in));
}

void SocketAddress::GetAddress(sockaddr & sa)
{
	memcpy(&sa, &sa_in, sizeof(sockaddr));
}

void SocketAddress::GetAddress(sockaddr_in & sa)
{
	memcpy(&sa, &sa_in, sizeof(sockaddr));
}




SocketAddress::~SocketAddress()
{
}

std::ostream & operator<<(std::ostream & os, SocketAddress & sa)
{
	//Si no ponemos unsigned
	os << unsigned(sa.sa_in.sin_addr.S_un.S_un_b.s_b1) << "." << unsigned(sa.sa_in.sin_addr.S_un.S_un_b.s_b2) << "." << unsigned(sa.sa_in.sin_addr.S_un.S_un_b.s_b3) << "." <<
		unsigned(sa.sa_in.sin_addr.S_un.S_un_b.s_b4) << ":" << sa.sa_in.sin_port;
	return os;
}

void SocketAddress::SetAddress(sockaddr sa)
{
	memcpy(&sa_in, &sa, sizeof(sockaddr));
}


int SocketAddress::SetAddress(const std::string & inString)
{
	/*inString.find_last_of
	Retorna la posici�n de la �ltima aparici�n de este caracter.
	Si no aparece, retorna string::npos.*/
	size_t pos = inString.find_last_of(':');
	std::string host, service;

	//npos es una constante que tiene el valor mayor posible que puede alcanzar una variable del tipo size_t.
	//size_t es un tipo num�rico tal que tiene el n�mero de bits sufientes para representar el n�mero m�s grande de tama�o de array que puede existir.
	//npos vale -1, porque como size_t es un tipo unsigned, -1 es el valor m�s grande que puede representarse (cuando da la vuelta en binario)
	if (pos != std::string::npos)
	{
		host = inString.substr(0, pos);
		service = inString.substr(pos + 1);
	}
	else
	{
		host = inString;
		//0 es el puerto por defecto
		service = "0";
	}

	addrinfo hint;
	//Fuerzo que toda la memoria que corresponde a hint valga 0. As� en getaddrinfo no habr� lugar a malentendidos
	//Recordamos que la funci�n de hint ser� la de filtrar los resultados que me retorne getaddrinfor.
	//En este caso, s�lo me retornar� aquellas direcciones que sean IPv4.
	memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_INET;

	//En result se almacenar� una lista de las direcciones IP:puerto que conincidan con mis
	//criterios de b�squeda
	addrinfo* result;
	int error = getaddrinfo(host.c_str(), service.c_str(), &hint, &result);

	if (error != 0 && result != nullptr)
	{
		freeaddrinfo(result);
		return -1;
	}
	//Recorro las direcciones de result en busca de una v�lida
	while (!result->ai_addr && result->ai_next)
	{
		result = result->ai_next;
	}
	if (!result->ai_addr)
	{
		freeaddrinfo(result);
		return -1;
	}

	//memcpy copia byte a byte la direcci�n v�lida de result en la direcci�n atributo 
	//de la clase.
	memcpy(&sa_in, result->ai_addr, sizeof(sockaddr_in));
	//sa_in.sin_port = htons(sa_in.sin_port);
	freeaddrinfo(result);

	return 0;
}

bool SocketAddress::operator==(const SocketAddress & sa)
{
	if (sa.sa_in.sin_addr.S_un.S_addr == sa_in.sin_addr.S_un.S_addr &&
		sa.sa_in.sin_port == sa_in.sin_port)
	{
		return true;
	}
	return false;
}
