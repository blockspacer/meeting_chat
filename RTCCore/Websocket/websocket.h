#pragma once

#include <stdint.h>

namespace core {

	enum class WebsocketState : int32_t
	{
		UnconnectedState = 0,

		ConnectingState = 2,

		ConnectedState = 3,

		ClosingState = 6
	};

	enum class WebsocketError : int32_t
	{
		ConnectionRefusedError,

		RemoteHostClosedError,

		HostNotFoundError,

		SocketAccessError,

		SocketResourceError,

		SocketTimeoutError,

		DatagramTooLargeError,

		NetworkError,

		AddressInUseError,

		SocketAddressNotAvailableError,

		UnsupportedSocketOperationError,

		ProxyAuthenticationRequiredError,

		SslHandshakeFailedError,

		UnfinishedSocketOperationError,

		ProxyConnectionRefusedError,

		ProxyConnectionClosedError,

		ProxyConnectionTimeoutError,

		ProxyNotFoundError,

		ProxyProtocolError,

		OperationError,

		SslInternalError,

		SslInvalidUserDataError,

		TemporaryError,

		UnknownSocketError
	};

	enum class WebsocketCloseCode : int32_t
	{
		CloseCodeNormal,

		CloseCodeGoingAway,

		CloseCodeProtocolError,

		CloseCodeDatatypeNotSupported,

		CloseCodeReserved1004,

		CloseCodeMissingStatusCode,

		CloseCodeAbnormalDisconnection,

		CloseCodeWrongDatatype,

		CloseCodePolicyViolated,

		CloseCodeTooMuchData,

		CloseCodeMissingExtension,

		CloseCodeBadOperation,

		CloseCodeTlsHandshakeFailed
	};
}