open Belt;

[%bs.raw {|require('./Checkbox.css')|}];

let component = ReasonReact.statelessComponent("Checkbox");

let make =
    (
      ~checked: bool,
      ~label: string,
      ~onChange: unit => unit,
      ~className="",
      _children,
    ) => {
  let uid = Global.generateUid();
  {
    ...component,
    render: _self =>
      <div className={j|Checkbox $(className)|j}>
        <input
          id=uid
          _type="checkbox"
          checked=(Js.Boolean.to_js_boolean(checked))
          onChange=(_event => onChange())
        />
        <label className="Checkbox__label" htmlFor=uid>
          (ReasonReact.stringToElement(label))
        </label>
      </div>,
  };
};
