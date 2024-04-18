const messageHandlers = {};
export const registerMessageHandler = (type, handler) => {
  messageHandlers[type] = handler;
};

export const initializeWebSocket = (url) => {
  const socket = new WebSocket(url);

  socket.onopen = () =>console.log("WebSocket connection opened");
  socket.onerror = (error) =>  console.error("WebSocket error", error);

  socket.onclose = () => {
    console.log("WebSocket connection closed, reconnecting in 1 second...");
    setTimeout(() => initializeWebSocket(url), 1000);
  };

  socket.onmessage = async (event) => {
    const message = JSON.parse(event.data);
    const type = message.type;

    if (messageHandlers[type]) {
      try {
        await messageHandlers[type](message);
      } catch (error) {
        console.error(error);
      }
    }
  };

  return socket;
};
