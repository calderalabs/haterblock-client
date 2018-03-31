[@bs.module "react-ladda"]
external laddaButton : ReasonReact.reactClass = "default";

let make =
    (
      ~onClick: 'a => 'b,
      ~className="",
      ~loading=false,
      ~disabled=false,
      children,
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=laddaButton,
    ~props={
      "onClick": onClick,
      "className": className,
      "loading": Js.Boolean.to_js_boolean(loading),
      "disabled": Js.Boolean.to_js_boolean(disabled),
      "data-style": "slide-left",
      "data-spinner-color": "#5c617b",
    },
    children,
  );
