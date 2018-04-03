type response;

type payload = {
  .
  "synced_at": Js.Null.t(string),
  "new_comment_count": int,
};

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

[@bs.module "phoenix/priv/static/phoenix"] [@bs.new]
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
  channel##on("syncing_updated", callback);
  channel##join();
};
