#include "NetworkClient.h"
#include <OutputMemoryStream.h>
#include <OutputMemoryBitStream.h>

NetworkClient::NetworkClient(std::string _addressServer, std::string _addressClient, std::string _nick):nick(_nick), networkState(NetworkState::UNINITIALIZED)
{
	saServer.SetAddress(_addressServer);
	SocketAddress myAddress;
	myAddress.SetAddress(_addressClient);
	int errBind = udpSocket.Bind(myAddress);
	int errBlock = udpSocket.NonBlocking(true);
	if (errBind > -1 && errBlock > -1)
	{
		//std::cout << "El estado de cliente pasa a SAYING HELLO" << std::endl;
		networkState = NetworkState::SAYINGHELLO;
	}
}

NetworkState NetworkClient::GetNetworkState()
{
	return networkState;
}

void NetworkClient::SetNetworkState(NetworkState _networkState)
{
	networkState = _networkState;
}

int NetworkClient::GetIdSquare()
{
	return idSquare;
}

void NetworkClient::SetIdSquare(int _idSquare)
{
	idSquare = _idSquare;
}

void NetworkClient::SayHello()
{
	clock_t time = clock();
	if (time > timeOfLastHello + FREQUENCY_SAYING_HELLO)
	{
		OutputMemoryBitStream ombs;
		ombs.Write(PacketType::PT_HELLO, 4);
		ombs.WriteString(nick);
		Send(ombs.GetBufferPtr(), ombs.GetByteLength());
		std::cout << "Envío hello" << std::endl;
		timeOfLastHello = time;
	}
}

void NetworkClient::SendMove(int _posSquareX, int _posSquareY, InputState& _inputState, InputStateList& _inputStateList) {
	clock_t time = clock();
	if (time > timeOfLastMove + FREQUENCY_SENDING_INPUTS) {
		if (_inputState.GetDeltaX() != 0 || _inputState.GetDeltaY() != 0) {
			//Hay movimiento que enviar --> Si es cero, me quedo donde estoy. No vale la pena perder tiempo en enviar y en esperar respuesta.
			_inputState.SetId(_inputStateList.GetCounter());
			std::vector<int> aMovesX = _inputState.GetMovesX();
			std::vector<int> aMovesY = _inputState.GetMovesY();
			_inputState.SetAbsolutePosition(_posSquareX, _posSquareY);
			_inputStateList.Add(_inputState);

			OutputMemoryBitStream ombs;
			ombs.Write(PacketType::PT_TRYPOSITION, 4);
			ombs.Write(_inputState.GetId());
			ombs.Write(aMovesX);
			ombs.Write(aMovesY);
			//std::cout << "Preparo un TRYPOSITION con id " << _inputState.GetId() << std::endl;
			//std::cout << "Contiene estos pasos: " << std::endl;
			/*for each (int var in aMoves) {
				std::cout << var << std::endl;

			}*/
			Send(ombs.GetBufferPtr(), ombs.GetByteLength());
			_inputState.ResetMove();
		} timeOfLastMove = time;

	}

}

void NetworkClient::SendShot(int _posSquareX, int _posSquareY, int _posShotX, int _posShotY, InputState& _inputState, InputStateList& _inputStateList) {
	OutputMemoryBitStream ombs;
	ombs.Write(PacketType::PT_SHOOT, 4);
	ombs.Write(_inputState.GetId());
	ombs.Write(_posSquareX, 10);
	ombs.Write(_posSquareY, 10);
	ombs.Write(_posShotX, 10);
	ombs.Write(_posShotY, 10);

	Send(ombs.GetBufferPtr(), ombs.GetByteLength());

}



void NetworkClient::Send(char* _message, int _size)
{
	//std::cout << "Envío: " << _message.c_str() << std::endl;
	udpSocket.SendTo(_message, _size, saServer);
}

int NetworkClient::Receive(std::string& _message)
{
	char buffer[MAX_BUFFER];
	SocketAddress from;
	int bytesReceived = Receive(buffer);
	if (bytesReceived > 0)
	{
		_message = buffer;
	}
	return bytesReceived;
}

int NetworkClient::Receive(char* _message)
{
	SocketAddress from;
	int bytesReceived = udpSocket.ReceiveFrom(_message, MAX_BUFFER, from);
	if (bytesReceived > 0)
	{
		if (bytesReceived < MAX_BUFFER)
		{
			_message[bytesReceived] = '\0';
		}
	}
	return bytesReceived;
}



NetworkClient::~NetworkClient()
{
}
