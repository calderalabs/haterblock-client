open Belt;

[%bs.raw {|require('./MessageBox.css')|}];

let component = ReasonReact.statelessComponent("MessageBox");

let make = (~type_: option(string)=?, children) => {
  ...component,
  render: _self => {
    let className =
      switch (type_) {
      | None => "MessageBox__inner"
      | Some(type_) => {j|MessageBox__inner MessageBox__inner--$type_|j}
      };
    <div className="MessageBox">
      (
        ReasonReact.createDomElement(
          "div",
          ~props={"className": className},
          children,
        )
      )
    </div>;
  },
};
