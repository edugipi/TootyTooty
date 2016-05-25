#include "ClientProxy.h"
#include "GameServerConstants.h"

ClientProxy::ClientProxy(): nick(""), littleSquare(330, 280), lastMoveWithError(false)
{
	
}

ClientProxy::ClientProxy(SocketAddress _saClient): littleSquare(330, 280), saClient(_saClient), lastMoveWithError(false)
{
}

ClientProxy::ClientProxy(std::string _nick, SocketAddress _saClient) : nick(_nick), littleSquare(330, 280), saClient(_saClient), lastMoveWithError(false)
{

}

ClientProxy::ClientProxy(const ClientProxy & _clientProxy):nick(_clientProxy.nick), littleSquare(_clientProxy.littleSquare)
{
	saClient = _clientProxy.saClient;
	lastMoveWithError = _clientProxy.lastMoveWithError;
}

bool ClientProxy::operator==(const ClientProxy & _clientProxy)
{
	return saClient == _clientProxy.saClient;
}

std::pair <int, int> ClientProxy::GetPositionSquare()
{
	return std::make_pair(littleSquare.GetPositionX(), littleSquare.GetPositionY());
}

void ClientProxy::SetPositionSquare(int posX, int posY)
{
	littleSquare.SetPosition(posX, posY);
}

std::pair <int, int> ClientProxy::ChangeMove(int _moveX, int _moveY) {
	int posX = littleSquare.GetPositionX();
	int posY = littleSquare.GetPositionY();
	int posAuxX = posX + _moveX;
	int posAuxY = posY + _moveY;
	if (posAuxX < 0) {
		posAuxX = 0;
		lastMoveWithError = true;

	} else if (posAuxX > 660) {
		posAuxX = 660;
		lastMoveWithError = true;

	} else {
		lastMoveWithError = false;

	}
	if (posAuxY < 0) {
		posAuxY = 0;
		lastMoveWithError = true;

	} else if (posAuxY > 560) {
		posAuxY = 560;
		lastMoveWithError = true;

	} else {
		lastMoveWithError = false;

	} littleSquare.SetPosition(posAuxX, posAuxY);
	
	return std::make_pair(posAuxX, posAuxY);
}

void ClientProxy::SetNick(std::string _nick)
{
	nick = _nick;
}

SocketAddress & ClientProxy::GetSocketAddress()
{
	return saClient;
}

bool ClientProxy::GetErrorLastMove()
{
	return lastMoveWithError;
}

ClientProxy::~ClientProxy()
{

}