open Belt;

[%bs.raw {|require('./Landing.css')|}];

let component = ReasonReact.statelessComponent("Landing");

let make = (~onLogin: unit => unit, _children) => {
  ...component,
  render: self =>
    <div className="Landing">
      <button onClick=(_event => onLogin()) className="Button Button--primary">
        (ReasonReact.stringToElement("Connect With Youtube To Get Started"))
      </button>
    </div>,
};
