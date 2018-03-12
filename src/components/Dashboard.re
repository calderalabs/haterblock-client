[%bs.raw {|require('./Dashboard.css')|}];

type state = {comments: option(array(Comment.t))};

type actions =
  | CommentsLoaded(array(Comment.t));

let component = ReasonReact.reducerComponent("Dasboard");

let make = (~user: User.t, _children) => {
  ...component,
  initialState: () => {comments: None},
  reducer: (action, _state: state) => {
    switch action {
    | CommentsLoaded(comments) => ReasonReact.NoUpdate
    };
  },
  render: ({ReasonReact.state}) =>
    <div className="Dashboard">
      <div> (ReasonReact.stringToElement(string_of_int(user.id))) </div>

      (
        switch state.comments {
        | None => ReasonReact.stringToElement("There are no comments to display")
        | Some(comments) =>
          (
            <div>
            (
              comments
              |> Array.map(
                    (comment) => <div>(ReasonReact.stringToElement(comment.Comment.body))</div>
                  )
              |> ReasonReact.arrayToElement
            )
            </div>
            )
        }
      )
    </div>
};
