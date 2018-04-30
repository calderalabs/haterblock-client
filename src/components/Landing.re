open Belt;

[%bs.raw {|require('./Landing.css')|}];

let component = ReasonReact.statelessComponent("Landing");

let make = (~onLogin: unit => unit, _children) => {
  ...component,
  render: (_) =>
    <div className="Landing">
      <div className="Landing__heading">
        <h1 className="Landing__title">
          (ReasonReact.stringToElement("Build a positive community"))
        </h1>
        <h2>
          (
            ReasonReact.stringToElement(
              "We use AI to help you identify and manage negative comments on your channel",
            )
          )
        </h2>
      </div>
      <div className="Landing__callToAction">
        <button
          onClick=(_event => onLogin()) className="Button Button--primary">
          (ReasonReact.stringToElement("Connect With Youtube"))
        </button>
      </div>
    </div>,
};
