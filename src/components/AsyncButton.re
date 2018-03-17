[%bs.raw {|require('./AsyncButton.css')|}];

type state = {loading: bool};

type action =
  | Loading
  | Loaded;

let component = ReasonReact.reducerComponent("AsyncButton");

let make = (~onClick: Callback.action(unit, unit), children) => {
  let onLaddaClick = (_event, {ReasonReact.send}) => {
    send(Loading);
    onClick((() => send(Loaded)) |> Callback.finally);
  };

  {
    ...component,
    initialState: () => {loading: false},
    reducer: (action, _state) =>
      switch action {
      | Loading => ReasonReact.Update({loading: true})
      | Loaded => ReasonReact.Update({loading: false})
      },
    render: ({ReasonReact.handle, state}) =>
      <LaddaButton onClick=(handle(onLaddaClick)) loading=state.loading> children </LaddaButton>
  };
};
