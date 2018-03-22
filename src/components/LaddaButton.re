[@bs.module "react-ladda"]
external laddaButton : ReasonReact.reactClass = "default";

let make = (~onClick: 'a => 'b, ~loading=false, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=laddaButton,
    ~props={
      "onClick": onClick,
      "loading": Js.Boolean.to_js_boolean(loading),
    },
    children,
  );
