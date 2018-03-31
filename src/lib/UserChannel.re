type response;

type payload = {. "syncing": bool};

type channel = {
  .
  [@bs.meth] "join": unit => channel,
  [@bs.meth] "receive": (string, response => unit) => channel,
  [@bs.meth] "on": (string, payload => unit) => unit,
};

type socketParams = {. "params": {. "token": string}};

type socket = {
  .
  [@bs.meth] "connect": unit => unit,
  [@bs.meth] "channel": string => channel,
};

[@bs.module "phoenix/assets/js/phoenix"] [@bs.new]
external socket : (string, socketParams) => socket = "Socket";

let join =
    (~user: UserData.User.t, ~token: string, ~callback: payload => unit) => {
  let userId = user.id;
  let baseHost = Api.baseHost;
  let socket =
    socket({j|ws://$(baseHost)/socket|j}, {
                                           "params": {
                                             "token": token,
                                           },
                                         });
  socket##connect();
  let channel = socket##channel({j|user:$userId|j});
  channel##on("user_changed", callback);
  channel##join()##receive("ok", response =>
    Js.log2("Joined successfully", response)
  )##receive(
    "error", response =>
    Js.log2("Failed to join", response)
  );
};
