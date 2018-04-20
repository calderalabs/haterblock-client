[@bs.val] external environment : string = "process.env.NODE_ENV";

[@bs.val] [@bs.scope "window.location"]
external reload : unit => unit = "reload";

[@bs.module "shortid"] external generateUid : unit => string = "generate";

let sessionTokenKey = "token";

let clearSessionToken = () =>
  Dom.Storage.(removeItem(sessionTokenKey, localStorage));
