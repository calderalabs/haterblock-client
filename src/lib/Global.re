[@bs.val] external environment : string = "process.env.NODE_ENV";

[@bs.val] [@bs.scope "window.location"]
external reload : unit => unit = "reload";

let clearLocalStorage = () => Dom.Storage.(removeItem("token", localStorage));
